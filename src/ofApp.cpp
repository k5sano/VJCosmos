#include "ofApp.h"
#include <cmath>
#include <algorithm>

// ── 定数 ──────────────────────────────────────────────────────────────────────
static const float SAMPLE_RATE       = 44100.0f;
static const float BIN_FREQ          = SAMPLE_RATE / ofApp::BUFFER_SIZE;

static const int BASS_LO =  0; static const int BASS_HI =   4;
static const int MID_LO  =  5; static const int MID_HI  =  46;
static const int HIGH_LO = 47; static const int HIGH_HI = 185;

static const float FLUX_ONSET_THRESH = 0.5f;

// ── MIDI CC 番号 ──────────────────────────────────────────────────────────────
static const int CC_DISSIPATION     = 21;
static const int CC_VEL_DISSIPATION = 22;
static const int CC_BLOOM_INTENSITY = 23;
static const int CC_BLOOM_THRESHOLD = 24;
static const int CC_BASS_SENS       = 25;
static const int CC_MID_SENS        = 26;
static const int CC_HIGH_SENS       = 27;
static const int CC_PLEXUS_THRESH   = 28;
static const int CC_ROT_SPEED       = 41;
static const int CC_DISPLACEMENT    = 42;
static const int CC_GLITCH_FREQ     = 43;
static const int CC_GRAVITY         = 44;
static const int CC_FLUID_SAT       = 45;
static const int CC_PLEXUS_SPEED    = 46;
static const int CC_RESERVED        = 47;
static const int CC_MASTER_BRIGHT   = 48;
static const int CC_UP    = 114;
static const int CC_DOWN  = 115;
static const int CC_LEFT  = 116;
static const int CC_RIGHT = 117;

// ── MIDI Note 番号 ────────────────────────────────────────────────────────────
static const int NOTE_FLUID_CLEAR   = 9;
static const int NOTE_PALETTE_NEXT  = 10;
static const int NOTE_GLITCH_BURST  = 11;
static const int NOTE_MESH_NEXT     = 12;
static const int NOTE_PLEXUS_TOGGLE = 25;
static const int NOTE_BLOOM_TOGGLE  = 26;
static const int NOTE_FREEZE_TOGGLE = 27;
static const int NOTE_SCREENSHOT    = 28;

// ── パレット/ジオメトリ名 ────────────────────────────────────────────────────
static const char* PALETTE_NAMES[] = {"WARM", "COOL", "MONO", "RAINBOW"};
static const char* GEOM_NAMES[]    = {"ICO", "CUBE", "SPHERE"};

static const int COORD_UPDATE_INTERVAL = 4;
static const int MAX_MESH_LABELS       = 12;
static const int MAX_PLEXUS_LABELS     = 8;

// ── ヘルパー ──────────────────────────────────────────────────────────────────
static ofFloatColor fromHue(float h, float s = 1.0f, float v = 1.0f) {
    ofFloatColor c; c.setHsb(h, s, v); return c;
}
static float asymSmooth(float prev, float raw, float atk, float rel) {
    return raw > prev ? prev + atk * (raw - prev) : prev * rel;
}

// ── Palette Color ─────────────────────────────────────────────────────────────
ofFloatColor ofApp::paletteColor(float t) {
    float tt = fmodf(fabsf(t), 1.0f);
    float sat = mFluidSaturation;
    switch (colorPalette) {
        case PALETTE_WARM:  return fromHue(ofLerp(0.0f,  0.12f, tt), sat);
        case PALETTE_COOL:  return fromHue(ofLerp(0.5f,  0.8f,  tt), sat);
        case PALETTE_MONO:  return fromHue(0, 0, ofLerp(0.5f, 1.0f, tt));
        case PALETTE_RAINBOW:
        default:            return fromHue(tt, sat);
    }
}

// ── Setup Typography ──────────────────────────────────────────────────────────
void ofApp::setupTypography() {
    typoMessages = {"FLOW", "WAVE", "PULSE", "BREAK", "RISE",
                    "FALL", "DREAM", "NOISE", "VOID", "ECHO"};
    typoMsgIndex = 0;
    typoTimer = 3.0f;

    ofTrueTypeFontSettings fs("verdana.ttf", 100);
    fs.antialiased = true;
    fs.contours    = true;
    typoFontOk = typoFont.load(fs);
    if (!typoFontOk) {
        ofLogWarning("TYPO") << "font load failed, using bitmap fallback";
    }

    int fw = ofGetWidth(), fh = ofGetHeight();
    typoFbo.allocate(fw, fh, GL_RGBA);
    typoFbo.begin(); ofClear(0,0,0,0); typoFbo.end();
}

// ── Setup Effects ─────────────────────────────────────────────────────────────
void ofApp::setupEffects() {
    fx[FX_KALEIDO].name = "Kaleido";
    fx[FX_KALEIDO].key  = 'k';
    fx[FX_KALEIDO].shader.load("shaders/passthrough.vert", "shaders/kaleidoscope.frag");

    fx[FX_WAVE].name = "Wave";
    fx[FX_WAVE].key  = '1';
    fx[FX_WAVE].shader.load("shaders/passthrough.vert", "shaders/wave.frag");

    fx[FX_GLITCH].name = "Glitch";
    fx[FX_GLITCH].key  = '2';
    fx[FX_GLITCH].shader.load("shaders/passthrough.vert", "shaders/glitch.frag");

    fx[FX_EDGE].name = "Edge";
    fx[FX_EDGE].key  = '3';
    fx[FX_EDGE].shader.load("shaders/passthrough.vert", "shaders/edge.frag");

    fx[FX_MONO].name = "Mono";
    fx[FX_MONO].key  = '4';
    fx[FX_MONO].shader.load("shaders/passthrough.vert", "shaders/mono.frag");

    fx[FX_MIRROR].name = "Mirror";
    fx[FX_MIRROR].key  = '5';
    fx[FX_MIRROR].shader.load("shaders/passthrough.vert", "shaders/mirror.frag");

    fx[FX_CRT].name = "CRT";
    fx[FX_CRT].key  = 'c';
    fx[FX_CRT].shader.load("shaders/passthrough.vert", "shaders/crt.frag");
}

// ── Setup ─────────────────────────────────────────────────────────────────────
void ofApp::setup() {
    ofSetFrameRate(60);
    ofBackground(0);
    ofSetEscapeQuitsApp(false);

    fluid.allocate(ofGetWidth(), ofGetHeight(), 0.5f);
    fluid.dissipation         = 0.999f;
    fluid.velocityDissipation = 0.99f;
    fluid.setGravity(ofVec2f(0.0f, -0.0098f));
    fluid.setUseObstacles(false);

    fft = ofxFft::create(BUFFER_SIZE, OF_FFT_WINDOW_HAMMING);
    prevAmplitude.assign(fft->getBinSize(), 0.0f);
    audioBuffer.assign(BUFFER_SIZE, 0.0f);

    int fw = ofGetWidth(), fh = ofGetHeight();
    renderFbo.allocate(fw, fh, GL_RGBA);
    brightFbo.allocate(fw, fh, GL_RGBA);
    blurHFbo.allocate(fw, fh, GL_RGBA);
    blurVFbo.allocate(fw, fh, GL_RGBA);
    sceneFbo.allocate(fw, fh, GL_RGBA);
    effectFboA.allocate(fw, fh, GL_RGBA);
    effectFboB.allocate(fw, fh, GL_RGBA);
    for (auto* f : {&renderFbo, &brightFbo, &blurHFbo, &blurVFbo, &sceneFbo, &effectFboA, &effectFboB}) {
        f->begin(); ofClear(0, 0, 0, 255); f->end();
    }
    brightnessShader.load("shaders/passthrough.vert", "shaders/brightness.frag");
    blurShader.load      ("shaders/passthrough.vert", "shaders/blur.frag");
    bloomShader.load     ("shaders/passthrough.vert", "shaders/bloom.frag");

    setupEffects();

    ofSoundStreamSettings ss;
    ss.setInListener(this);
    ss.sampleRate = (int)SAMPLE_RATE; ss.numInputChannels = 1;
    ss.numOutputChannels = 0; ss.bufferSize = BUFFER_SIZE;
    soundStream.setup(ss);

    setupWireframe();
    setupPlexus();
    setupTypography();

    // ── MIDI ──────────────────────────────────────────────────────────────────
    int numPorts = midiIn.getNumInPorts();
    ofLogNotice("MIDI") << "--- " << numPorts << " MIDI input ports ---";
    for (int i = 0; i < numPorts; i++) {
        ofLogNotice("MIDI") << "  [" << i << "] " << midiIn.getInPortName(i);
    }

    int targetPort = -1;
    for (int i = 0; i < numPorts; i++) {
        if (midiIn.getInPortName(i).find("Launch Control") != std::string::npos) {
            targetPort = i;
            break;
        }
    }

    if (targetPort >= 0) {
        midiIn.openPort(targetPort);
        ofLogNotice("MIDI") << "opened Launch Control on port " << targetPort
                            << ": " << midiIn.getName();
    } else if (numPorts > 0) {
        ofLogWarning("MIDI") << "Launch Control not found among " << numPorts << " ports:";
        for (int i = 0; i < numPorts; i++) {
            ofLogWarning("MIDI") << "  [" << i << "] " << midiIn.getInPortName(i);
        }
        ofLogWarning("MIDI") << "running without MIDI";
    } else {
        ofLogNotice("MIDI") << "no MIDI ports found";
    }
    midiIn.addListener(this);
    midiIn.ignoreTypes(true, true, true);

    // ── OSC ────────────────────────────────────────────────────────────────────
    oscReceiver.setup(OSC_PORT);
    ofLogNotice("OSC") << "Listening on port " << OSC_PORT;

    // ── Syphon ────────────────────────────────────────────────────────────────
    syphonServer.setName("VJCosmos");

    // ── オーディオデバイス一覧 ────────────────────────────────────────────────
    auto allDevices = soundStream.getDeviceList(ofSoundDevice::Api::DEFAULT);
    for (auto& d : allDevices) {
        if (d.inputChannels > 0) inputDevices.push_back(d);
    }
    ofLogNotice("AUDIO") << inputDevices.size() << " input devices found";
    for (int i = 0; i < (int)inputDevices.size(); i++) {
        ofLogNotice("AUDIO") << "  [" << i << "] " << inputDevices[i].name;
    }

    // ── プリセットディレクトリ ────────────────────────────────────────────────
    ofDirectory::createDirectory("presets", true, true);
}

// ── AudioIn (audio thread) ────────────────────────────────────────────────────
void ofApp::audioIn(ofSoundBuffer& input) {
    std::lock_guard<std::mutex> lock(audioMutex);
    const auto& buf = input.getBuffer();
    size_t n = std::min(buf.size(), (size_t)BUFFER_SIZE);
    audioBuffer.assign(buf.begin(), buf.begin() + n);
    float sq = 0; for (size_t i = 0; i < n; i++) sq += buf[i] * buf[i];
    rawRms = sqrtf(sq / n);
}

// ── MIDI Callback (MIDI thread) ──────────────────────────────────────────────
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
    std::lock_guard<std::mutex> lock(midiMutex);
    lastMidiMsg = msg;

    if (msg.status == MIDI_CONTROL_CHANGE) {
        midiCC[msg.control] = msg.value / 127.0f;
        if (msg.value == 127) {
            if (msg.control == CC_UP)   gravityUp   = !gravityUp;
            if (msg.control == CC_DOWN) gravityDown  = !gravityDown;
        }
    } else if (msg.status == MIDI_NOTE_ON && msg.velocity > 0) {
        midiNoteOn[msg.pitch] = true;
        if (msg.pitch == NOTE_FLUID_CLEAR)   midiTrigFluidClear  = true;
        if (msg.pitch == NOTE_PALETTE_NEXT)  midiTrigPaletteNext = true;
        if (msg.pitch == NOTE_GLITCH_BURST)  midiTrigGlitchBurst = true;
        if (msg.pitch == NOTE_MESH_NEXT)     midiTrigMeshNext    = true;
        if (msg.pitch == NOTE_SCREENSHOT)    midiTrigScreenshot  = true;
        if (msg.pitch == NOTE_PLEXUS_TOGGLE) plexusVisible = !plexusVisible;
        if (msg.pitch == NOTE_BLOOM_TOGGLE)  bloomEnabled  = !bloomEnabled;
        if (msg.pitch == NOTE_FREEZE_TOGGLE) frozen        = !frozen;
    } else if (msg.status == MIDI_NOTE_OFF ||
               (msg.status == MIDI_NOTE_ON && msg.velocity == 0)) {
        midiNoteOn[msg.pitch] = false;
    }
}

// ── Apply MIDI Parameters ────────────────────────────────────────────────────
void ofApp::applyMidiParams(float dt) {
    float targets[16];
    bool trigClear, trigPalette, trigGlitch, trigMesh, trigShot;
    bool windLeft, windRight;
    {
        std::lock_guard<std::mutex> lock(midiMutex);
        auto mapCC = [&](int ccNum, float lo, float hi, float def) -> float {
            auto it = midiCC.find(ccNum);
            return it != midiCC.end() ? ofLerp(lo, hi, it->second) : def;
        };

        targets[0]  = mapCC(CC_DISSIPATION,      0.9f,   1.0f,   0.999f);
        targets[1]  = mapCC(CC_VEL_DISSIPATION,   0.8f,   1.0f,   0.99f);
        targets[2]  = mapCC(CC_BLOOM_INTENSITY,    0.0f,   2.0f,   0.5f);
        targets[3]  = mapCC(CC_BLOOM_THRESHOLD,    0.1f,   0.8f,   0.3f);
        targets[4]  = mapCC(CC_BASS_SENS,          0.5f,   3.0f,   1.0f);
        targets[5]  = mapCC(CC_MID_SENS,           0.5f,   3.0f,   1.0f);
        targets[6]  = mapCC(CC_HIGH_SENS,          0.5f,   3.0f,   1.0f);
        targets[7]  = mapCC(CC_PLEXUS_THRESH,     50.0f, 300.0f, 150.0f);
        targets[8]  = mapCC(CC_ROT_SPEED,          0.0f,  60.0f,  15.0f);
        targets[9]  = mapCC(CC_DISPLACEMENT,       0.0f,   2.0f,   1.0f);
        targets[10] = mapCC(CC_GLITCH_FREQ,        0.0f,   3.0f,   1.0f);
        targets[11] = mapCC(CC_GRAVITY,           -0.05f,  0.05f, -0.0098f);
        targets[12] = mapCC(CC_FLUID_SAT,          0.0f,   1.0f,   1.0f);
        targets[13] = mapCC(CC_PLEXUS_SPEED,       0.1f,   3.0f,   1.0f);
        targets[14] = mapCC(CC_RESERVED,           0.0f,   1.0f,   0.0f);
        targets[15] = mapCC(CC_MASTER_BRIGHT,      0.5f,   1.5f,   1.0f);

        trigClear   = midiTrigFluidClear;   midiTrigFluidClear   = false;
        trigPalette = midiTrigPaletteNext;  midiTrigPaletteNext  = false;
        trigGlitch  = midiTrigGlitchBurst;  midiTrigGlitchBurst  = false;
        trigMesh    = midiTrigMeshNext;     midiTrigMeshNext     = false;
        trigShot    = midiTrigScreenshot;   midiTrigScreenshot   = false;

        windLeft  = midiCC.count(CC_LEFT)  && midiCC[CC_LEFT]  > 0.5f;
        windRight = midiCC.count(CC_RIGHT) && midiCC[CC_RIGHT] > 0.5f;
    }

    // OSC parameter overrides (take priority over MIDI)
    auto oscOverride = [&](const std::string& name, float& target) {
        auto it = oscParams.find(name);
        if (it != oscParams.end()) target = it->second;
    };
    oscOverride("dissipation",      targets[0]);
    oscOverride("velDissipation",   targets[1]);
    oscOverride("bloomIntensity",   targets[2]);
    oscOverride("bloomThreshold",   targets[3]);
    oscOverride("bassSens",         targets[4]);
    oscOverride("midSens",          targets[5]);
    oscOverride("highSens",         targets[6]);
    oscOverride("rotSpeed",         targets[8]);
    oscOverride("displacement",     targets[9]);
    oscOverride("glitchFreq",       targets[10]);
    oscOverride("gravity",          targets[11]);
    oscOverride("fluidSaturation",  targets[12]);
    oscOverride("plexusSpeed",      targets[13]);
    oscOverride("masterBrightness", targets[15]);

    // Boolean/int params from OSC
    auto oscBoolOverride = [&](const std::string& name, bool& target) {
        auto it = oscParams.find(name);
        if (it != oscParams.end()) target = it->second > 0.5f;
    };
    oscBoolOverride("fxKaleido", fx[FX_KALEIDO].enabled);
    oscBoolOverride("fxCrt",     fx[FX_CRT].enabled);
    oscBoolOverride("fxWave",    fx[FX_WAVE].enabled);
    oscBoolOverride("fxGlitch",  fx[FX_GLITCH].enabled);
    oscBoolOverride("fxEdge",    fx[FX_EDGE].enabled);
    oscBoolOverride("fxMono",    fx[FX_MONO].enabled);
    oscBoolOverride("fxMirror",  fx[FX_MIRROR].enabled);

    // Effect parameters from OSC
    auto oscFloat = [&](const std::string& name, float& target) {
        auto it = oscParams.find(name);
        if (it != oscParams.end()) target = it->second;
    };
    oscFloat("waveAmplitude", waveAmplitude);
    oscFloat("waveSpeed",     waveSpeed);
    oscFloat("edgeStrength",  edgeStrength);
    oscFloat("edgeMix",       edgeMix);
    oscFloat("monoMode",      monoMode);
    oscFloat("mirrorMode",    mirrorMode);
    {
        auto it = oscParams.find("colorPalette");
        if (it != oscParams.end())
            colorPalette = (ColorPalette)ofClamp((int)it->second, 0, PALETTE_COUNT - 1);
    }

    const float sm = 0.1f;
    mDissipation     = ofLerp(mDissipation,     targets[0],  sm);
    mVelDissipation  = ofLerp(mVelDissipation,  targets[1],  sm);
    mBloomIntensity  = ofLerp(mBloomIntensity,  targets[2],  sm);
    mBloomThreshold  = ofLerp(mBloomThreshold,  targets[3],  sm);
    mBassSens        = ofLerp(mBassSens,        targets[4],  sm);
    mMidSens         = ofLerp(mMidSens,         targets[5],  sm);
    mHighSens        = ofLerp(mHighSens,        targets[6],  sm);
    mPlexusThresh    = ofLerp(mPlexusThresh,    targets[7],  sm);
    mRotSpeed        = ofLerp(mRotSpeed,        targets[8],  sm);
    mDisplacement    = ofLerp(mDisplacement,    targets[9],  sm);
    mGlitchFreq      = ofLerp(mGlitchFreq,     targets[10], sm);
    mGravity         = ofLerp(mGravity,         targets[11], sm);
    mFluidSaturation = ofLerp(mFluidSaturation, targets[12], sm);
    mPlexusSpeed     = ofLerp(mPlexusSpeed,     targets[13], sm);
    mReserved        = ofLerp(mReserved,        targets[14], sm);
    mMasterBright    = ofLerp(mMasterBright,    targets[15], sm);

    fluid.dissipation         = mDissipation;
    fluid.velocityDissipation = mVelDissipation;

    float gravMag = std::max(fabsf(mGravity), 0.0001f);
    if (gravityUp && !gravityDown)        fluid.setGravity(ofVec2f(0, -gravMag));
    else if (gravityDown && !gravityUp)   fluid.setGravity(ofVec2f(0, gravMag));
    else if (!gravityUp && !gravityDown)  fluid.setGravity(ofVec2f(0, mGravity));
    else                                  fluid.setGravity(ofVec2f(0, 0));

    float w = ofGetWidth(), h = ofGetHeight();
    float ft = ofGetElapsedTimef();
    if (windLeft)
        fluid.addTemporalForce(ofVec2f(w*0.9f, h*0.5f), ofVec2f(-200, 0),
                               paletteColor(ft*0.1f), 30.0f, 5.0f, 0.5f);
    if (windRight)
        fluid.addTemporalForce(ofVec2f(w*0.1f, h*0.5f), ofVec2f(200, 0),
                               paletteColor(ft*0.1f+0.5f), 30.0f, 5.0f, 0.5f);

    if (trigClear)   fluid.clear();
    if (trigPalette) colorPalette = (ColorPalette)((colorPalette + 1) % PALETTE_COUNT);
    if (trigGlitch)  { spawnPolygons((int)ofRandom(10, 25), true); flashAlpha = 30.0f; }
    if (trigMesh)    { meshGeometry = (MeshGeometry)((meshGeometry + 1) % GEOM_COUNT); rebuildMesh(); }
    if (trigShot)    ofSaveScreen("screenshot_" + ofGetTimestampString() + ".png");
}

// ── OSC Message Processing ────────────────────────────────────────────────────
void ofApp::processOscMessages() {
    while (oscReceiver.hasWaitingMessages()) {
        ofxOscMessage msg;
        oscReceiver.getNextMessage(msg);
        string addr = msg.getAddress();
        oscMsgCount++;
        lastOscAddr = addr;
        lastOscTime = ofGetElapsedTimef();

        if      (addr == "/vjcosmos/bass")     { oscBass     = msg.getArgAsFloat(0); oscFftActive = true; oscFftTimeout = 2.0f; oscFftMsgCount++; }
        else if (addr == "/vjcosmos/mid")      { oscMid      = msg.getArgAsFloat(0); oscFftActive = true; oscFftTimeout = 2.0f; oscFftMsgCount++; }
        else if (addr == "/vjcosmos/high")     { oscHigh     = msg.getArgAsFloat(0); oscFftActive = true; oscFftTimeout = 2.0f; oscFftMsgCount++; }
        else if (addr == "/vjcosmos/rms")      { oscRms      = msg.getArgAsFloat(0); oscFftActive = true; oscFftTimeout = 2.0f; oscFftMsgCount++; }
        else if (addr == "/vjcosmos/centroid") { oscCentroid  = msg.getArgAsFloat(0); oscFftActive = true; oscFftTimeout = 2.0f; oscFftMsgCount++; }
        else if (addr == "/vjcosmos/flux")     { oscFlux     = msg.getArgAsFloat(0); oscFftActive = true; oscFftTimeout = 2.0f; oscFftMsgCount++; }
        else if (addr == "/vjcosmos/onset")    { oscOnset    = msg.getArgAsFloat(0) > 0.5f; oscFftActive = true; oscFftTimeout = 2.0f; oscFftMsgCount++; }
        else if (addr.length() > 17 && addr.substr(0, 17) == "/vjcosmos/param/") {
            string paramName = addr.substr(17);
            if (msg.getNumArgs() > 0) {
                float val = msg.getArgAsFloat(0);
                oscParams[paramName] = val;
                oscParamMsgCount++;
                oscParamLog[paramName] = { val, ofGetElapsedTimef(), true };
                // Log first time or value change (throttled)
                static std::map<std::string, float> lastLogged;
                if (lastLogged.find(paramName) == lastLogged.end() || fabsf(lastLogged[paramName] - val) > 0.001f) {
                    ofLogNotice("OSC") << "param: " << paramName << " = " << val;
                    lastLogged[paramName] = val;
                }
            }
        }
    }

    float dt = ofGetLastFrameTime();
    if (oscFftActive) {
        oscFftTimeout -= dt;
        if (oscFftTimeout <= 0) oscFftActive = false;
    }
}

// ── Rebuild Mesh ──────────────────────────────────────────────────────────────
void ofApp::rebuildMesh() {
    switch (meshGeometry) {
        case GEOM_ICOSPHERE: {
            ofIcoSpherePrimitive ico; ico.setRadius(150.0f); ico.setResolution(2);
            sphereMesh = ico.getMesh(); break;
        }
        case GEOM_CUBE: {
            ofBoxPrimitive box; box.set(200.0f); box.setResolution(4);
            sphereMesh = box.getMesh(); break;
        }
        case GEOM_SPHERE: {
            ofSpherePrimitive sph; sph.setRadius(150.0f); sph.setResolution(16);
            sphereMesh = sph.getMesh(); break;
        }
        default: break;
    }
    origVerts = sphereMesh.getVertices();
    origNorms.resize(origVerts.size());
    for (size_t i = 0; i < origVerts.size(); i++) {
        float len = glm::length(origVerts[i]);
        origNorms[i] = len > 0.001f ? glm::normalize(origVerts[i]) : glm::vec3(0, 1, 0);
    }
    meshLabels.clear();
}

// ── Window Resized ────────────────────────────────────────────────────────────
void ofApp::windowResized(int w, int h) {
    if (w <= 0 || h <= 0) return;
    float oldW = renderFbo.getWidth(), oldH = renderFbo.getHeight();
    if (oldW > 0 && oldH > 0) {
        float sx = (float)w / oldW, sy = (float)h / oldH;
        for (auto& p : plexusParticles) { p.pos.x *= sx; p.pos.y *= sy; }
    }
    reallocateFbos(w, h);
    fluid.allocate(w, h, 0.5f);
    fluid.dissipation = mDissipation;
    fluid.velocityDissipation = mVelDissipation;
    fluid.setUseObstacles(false);
    typoFbo.allocate(w, h, GL_RGBA);
    typoFbo.begin(); ofClear(0,0,0,0); typoFbo.end();
}

void ofApp::reallocateFbos(int w, int h) {
    renderFbo.allocate(w, h, GL_RGBA);
    brightFbo.allocate(w, h, GL_RGBA);
    blurHFbo.allocate(w, h, GL_RGBA);
    blurVFbo.allocate(w, h, GL_RGBA);
    sceneFbo.allocate(w, h, GL_RGBA);
    effectFboA.allocate(w, h, GL_RGBA);
    effectFboB.allocate(w, h, GL_RGBA);
    for (auto* f : {&renderFbo, &brightFbo, &blurHFbo, &blurVFbo, &sceneFbo, &effectFboA, &effectFboB}) {
        f->begin(); ofClear(0, 0, 0, 255); f->end();
    }
}

// ── KeyPressed ────────────────────────────────────────────────────────────────
void ofApp::keyPressed(int key) {
    if (key == 'r' || key == 'R') fluidRunning = !fluidRunning;
    if (key == 'f' || key == 'F') {
        isFullscreen = !isFullscreen;
        ofSetFullscreen(isFullscreen);
        if (isFullscreen) ofHideCursor(); else ofShowCursor();
    }
    if (key == OF_KEY_ESC && isFullscreen) {
        isFullscreen = false; ofSetFullscreen(false); ofShowCursor();
    }
    if (key == 'd' || key == 'D') showDebug     = !showDebug;
    if (key == 'o' || key == 'O') showOscDebug  = !showOscDebug;
    if (key == 't' || key == 'T') showCoordText = !showCoordText;
    if (key == 'h' || key == 'H') showHelp      = !showHelp;
    if (key == 'v' || key == 'V') typoEnabled   = !typoEnabled;
    if (key == 'a' || key == 'A') {
        if (!inputDevices.empty()) {
            currentAudioDevice = (currentAudioDevice + 1) % inputDevices.size();
            switchAudioDevice(currentAudioDevice);
        }
    }

    // エフェクトレイヤーのトグル
    for (int i = 0; i < FX_COUNT; i++) {
        if (key == fx[i].key || key == toupper(fx[i].key))
            fx[i].enabled = !fx[i].enabled;
    }

    // プリセット: Cmd+S = quick save, Cmd+L = quick load
    bool cmd = ofGetKeyPressed(OF_KEY_COMMAND);
    if (cmd && (key == 's' || key == 'S')) { savePreset("quick"); return; }
    if (cmd && (key == 'l' || key == 'L')) { loadPreset("quick"); return; }

    // Cmd+1〜8: save preset_1〜8
    if (cmd && key >= '1' && key <= '8') {
        savePreset("preset_" + ofToString(key - '0'));
        return;
    }
    // Shift+1〜8: load preset_1〜8 (Shift+数字のキーコード)
    const string shiftDigits = "!@#$%^&*";
    for (int i = 0; i < 8; i++) {
        if (key == shiftDigits[i]) {
            loadPreset("preset_" + ofToString(i + 1));
            return;
        }
    }
}

// ── Typography Update ─────────────────────────────────────────────────────────
void ofApp::updateTypography(float dt, float w, float h) {
    if (!typoEnabled) return;
    typoTimer -= dt;
    bool bassOnset = onsetRising && sBass > 0.15f;
    if (typoTimer <= 0 || bassOnset) {
        typoInject = true;
        typoTimer  = ofRandom(3.0f, 5.0f);
        typoPos    = ofVec2f(w * 0.5f + ofRandom(-100, 100),
                             h * 0.5f + ofRandom(-100, 100));
    }
}

void ofApp::injectTypography(float w, float h) {
    if (!typoInject) return;
    typoInject = false;

    const string& msg = typoMessages[typoMsgIndex];
    typoMsgIndex = (typoMsgIndex + 1) % typoMessages.size();

    ofFloatColor fc = paletteColor(ofRandom(0.0f, 1.0f));
    ofColor col(fc.r * 255, fc.g * 255, fc.b * 255, 255);

    typoFbo.begin();
    ofClear(0, 0, 0, 255);
    ofSetColor(col);
    if (typoFontOk) {
        ofRectangle bb = typoFont.getStringBoundingBox(msg, 0, 0);
        typoFont.drawString(msg, typoPos.x - bb.width * 0.5f, typoPos.y + bb.height * 0.5f);
    } else {
        ofPushMatrix();
        ofTranslate(typoPos.x - msg.length() * 12, typoPos.y);
        ofScale(6.0f);
        ofDrawBitmapString(msg, 0, 0);
        ofPopMatrix();
    }
    typoFbo.end();

    fluid.addColor(typoFbo, 1.0f);

    float force = 30.0f + sBass * 50.0f;
    for (int i = 0; i < 8; i++) {
        float a = i * TWO_PI / 8 + ofRandom(-0.3f, 0.3f);
        fluid.addTemporalForce(typoPos, ofVec2f(cosf(a)*force, sinf(a)*force),
                               paletteColor(ofRandom(1.0f)), 20.0f, 5.0f, 0.3f);
    }
}

// ── Update Effect Parameters (audio-reactive) ────────────────────────────────
void ofApp::updateEffectParams(float dt) {
    float effBass = sBass * mBassSens;

    // Kaleidoscope
    if (fx[FX_KALEIDO].enabled) {
        kaleidoRotBoost = ofLerp(kaleidoRotBoost, 0.0f, 0.05f);
        if (onsetRising) kaleidoRotBoost = 2.0f;
        kaleidoRotation += (0.3f + sCentroid * 0.5f + kaleidoRotBoost) * dt;
    }

    // CRT
    if (fx[FX_CRT].enabled) {
        float targetChrom = crtChromatic + effBass * 3.0f;
        if (onsetRising) targetChrom = 10.0f;
        crtChromaticCur = ofLerp(crtChromaticCur, targetChrom, 0.3f);
    }

    // Wave
    if (fx[FX_WAVE].enabled) {
        waveSpeedCur = ofLerp(waveSpeedCur, waveSpeed, 0.1f);
        if (onsetRising) waveSpeedCur = 5.0f;
    }

    // Glitch
    {
        float target = effBass * 0.2f;
        if (onsetRising) target = 0.8f;
        glitchIntensityCur = ofLerp(glitchIntensityCur, target, 0.3f);
    }

    // Edge
    // edgeStrength は bass に連動
    // edgeMix はそのまま

    // Mono
    {
        float target = monoIntensity;
        if (onsetRising) target = 0.0f; // onset で一瞬色が戻る
        monoIntensityCur = ofLerp(monoIntensityCur, target, 0.2f);
        if (!onsetRising) monoIntensityCur = ofLerp(monoIntensityCur, monoIntensity, 0.05f);
    }
}

// ── Apply Effect Chain (ping-pong FBO) ────────────────────────────────────────
void ofApp::applyEffectChain(float w, float h) {
    float effBass = sBass * mBassSens;
    float t = ofGetElapsedTimef();

    // sceneFbo → effectFboA にコピー
    effectFboA.begin(); ofClear(0,0,0,255);
    ofSetColor(255);
    sceneFbo.draw(0,0,w,h);
    effectFboA.end();

    ofFbo* src = &effectFboA;
    ofFbo* dst = &effectFboB;

    auto applyShader = [&](int fxIdx, std::function<void(ofShader&)> setUniforms) {
        if (!fx[fxIdx].enabled) return;
        dst->begin(); ofClear(0,0,0,255);
        fx[fxIdx].shader.begin();
        fx[fxIdx].shader.setUniformTexture("tex", src->getTexture(), 0);
        setUniforms(fx[fxIdx].shader);
        ofSetColor(255);
        src->draw(0,0,w,h);
        fx[fxIdx].shader.end();
        dst->end();
        std::swap(src, dst);
    };

    // 0: Kaleidoscope
    applyShader(FX_KALEIDO, [&](ofShader& s) {
        s.setUniform2f("resolution", w, h);
        s.setUniform1i("segments", kaleidoSegments);
        s.setUniform1f("rotation", kaleidoRotation);
        s.setUniform1f("zoom", 1.0f);
    });

    // 1: Wave
    applyShader(FX_WAVE, [&](ofShader& s) {
        s.setUniform2f("resolution", w, h);
        s.setUniform1f("time", t);
        s.setUniform1f("amplitude", waveAmplitude + effBass * 30.0f);
        s.setUniform1f("frequency", waveFrequency);
        s.setUniform1f("speed", waveSpeedCur);
    });

    // 2: Glitch
    applyShader(FX_GLITCH, [&](ofShader& s) {
        s.setUniform2f("resolution", w, h);
        s.setUniform1f("time", t);
        s.setUniform1f("intensity", glitchIntensityCur);
        s.setUniform1f("blockSize", glitchBlockSize);
    });

    // 3: Edge
    applyShader(FX_EDGE, [&](ofShader& s) {
        s.setUniform2f("resolution", w, h);
        s.setUniform1f("strength", edgeStrength + effBass * 0.5f);
        s.setUniform1f("mixAmount", edgeMix);
    });

    // 4: Mono
    applyShader(FX_MONO, [&](ofShader& s) {
        s.setUniform1f("mode", monoMode);
        s.setUniform1f("intensity", monoIntensityCur);
    });

    // 5: Mirror
    applyShader(FX_MIRROR, [&](ofShader& s) {
        s.setUniform2f("resolution", w, h);
        s.setUniform1f("mode", mirrorMode);
    });

    // 6: CRT (常に最後)
    applyShader(FX_CRT, [&](ofShader& s) {
        s.setUniform2f("resolution", w, h);
        s.setUniform1f("scanlineSpacing", 3.0f);
        s.setUniform1f("curvature", 0.03f);
        s.setUniform1f("chromatic", crtChromaticCur);
        s.setUniform1f("vignetteAmount", 0.3f);
        s.setUniform1f("noiseAmount", 0.03f + sHigh * 0.05f);
        s.setUniform1f("time", t);
    });

    // 最終結果を画面に描画
    ofSetColor(255);
    src->draw(0,0,w,h);
}

// ── Update Coordinate Labels ──────────────────────────────────────────────────
void ofApp::updateCoordLabels(float dt) {
    if (onsetRising) coordGlitchTimer = 0.08f;
    coordGlitchTimer = std::max(0.0f, coordGlitchTimer - dt);
    coordFrameCounter++;
    bool updateFrame = (coordFrameCounter % COORD_UPDATE_INTERVAL == 0);

    if (updateFrame && !meshDisplacements.empty()) {
        int nv = (int)meshDisplacements.size();
        int maxLabels = std::min(MAX_MESH_LABELS, nv);
        std::vector<std::pair<float, int>> ranked;
        ranked.reserve(nv);
        for (int i = 0; i < nv; i++) ranked.push_back({meshDisplacements[i], i});
        std::partial_sort(ranked.begin(), ranked.begin() + maxLabels, ranked.end(),
                          [](const auto& a, const auto& b){ return a.first > b.first; });
        std::set<int> activeSet;
        for (int i = 0; i < maxLabels; i++) activeSet.insert(ranked[i].second);
        for (auto& [idx, label] : meshLabels)
            label.targetAlpha = activeSet.count(idx) ? 1.0f : 0.0f;
        for (int idx : activeSet) {
            if (meshLabels.find(idx) == meshLabels.end())
                meshLabels[idx] = {0, 1.0f, "", ofVec2f(0)};
            else meshLabels[idx].targetAlpha = 1.0f;
        }
        float normScale = 200.0f;
        for (auto& [idx, label] : meshLabels) {
            if (idx < (int)meshScreenPos.size()) label.pos = meshScreenPos[idx];
            if (idx < (int)sphereMesh.getNumVertices()) {
                const auto& v = sphereMesh.getVertices()[idx];
                label.text = "x:" + ofToString(v.x/normScale,3)
                           + " y:" + ofToString(v.y/normScale,3)
                           + " z:" + ofToString(v.z/normScale,3);
            }
        }
    }

    if (updateFrame && !plexusConnCounts.empty() && plexusVisible) {
        int np = (int)plexusConnCounts.size();
        int maxLabels = std::min(MAX_PLEXUS_LABELS, np);
        std::vector<std::pair<int, int>> ranked;
        ranked.reserve(np);
        for (int i = 0; i < np; i++) ranked.push_back({plexusConnCounts[i], i});
        std::partial_sort(ranked.begin(), ranked.begin() + maxLabels, ranked.end(),
                          [](const auto& a, const auto& b){ return a.first > b.first; });
        std::set<int> activeSet;
        for (int i = 0; i < maxLabels; i++)
            if (ranked[i].first > 0) activeSet.insert(ranked[i].second);
        for (auto& [idx, label] : plexusLabels)
            label.targetAlpha = activeSet.count(idx) ? 1.0f : 0.0f;
        for (int idx : activeSet) {
            if (plexusLabels.find(idx) == plexusLabels.end())
                plexusLabels[idx] = {0, 1.0f, "", ofVec2f(0)};
            else plexusLabels[idx].targetAlpha = 1.0f;
        }
        float pw = ofGetWidth(), ph = ofGetHeight();
        for (auto& [idx, label] : plexusLabels) {
            if (idx < (int)plexusParticles.size()) {
                const auto& p = plexusParticles[idx];
                label.pos = ofVec2f(p.pos.x, p.pos.y);
                char buf[32];
                snprintf(buf, sizeof(buf), "[%03d] %.2f, %.2f", idx, p.pos.x/pw, p.pos.y/ph);
                label.text = buf;
            }
        }
    }

    if (!plexusVisible)
        for (auto& [idx, label] : plexusLabels) label.targetAlpha = 0;

    float fadeIn = dt / 0.3f, fadeOut = dt / 0.2f;
    auto fade = [&](std::map<int, CoordLabel>& labels) {
        for (auto it = labels.begin(); it != labels.end(); ) {
            auto& l = it->second;
            l.alpha = l.targetAlpha > l.alpha
                ? std::min(l.targetAlpha, l.alpha + fadeIn)
                : std::max(0.0f, l.alpha - fadeOut);
            if (l.alpha <= 0 && l.targetAlpha <= 0) it = labels.erase(it);
            else ++it;
        }
    };
    fade(meshLabels);
    fade(plexusLabels);
}

// ── Draw Coordinate Labels ────────────────────────────────────────────────────
void ofApp::drawCoordLabels() {
    if (!showCoordText || coordGlitchTimer > 0) return;
    float bassVib = sBass * 2.0f;
    float cx = ofGetWidth() * 0.5f, cy = ofGetHeight() * 0.5f;
    ofPushStyle();
    for (const auto& [idx, label] : meshLabels) {
        if (label.alpha <= 0.01f) continue;
        float vx = bassVib > 0.1f ? ofRandom(-bassVib, bassVib) : 0;
        float vy = bassVib > 0.1f ? ofRandom(-bassVib, bassVib) : 0;
        float leaderLen = 20.0f + (idx * 7 % 21);
        ofVec2f dir = label.pos - ofVec2f(cx, cy);
        if (dir.length() > 1.0f) dir.normalize(); else dir.set(1, 0);
        ofVec2f tp = label.pos + dir * leaderLen + ofVec2f(vx, vy);
        ofSetColor(255, 255, 255, (int)(label.alpha * 80));
        ofSetLineWidth(1.0f);
        ofDrawLine(label.pos.x, label.pos.y, tp.x, tp.y);
        ofSetColor(255, 255, 255, (int)(label.alpha * 150));
        ofDrawBitmapString(label.text, tp.x + 3, tp.y);
    }
    for (const auto& [idx, label] : plexusLabels) {
        if (label.alpha <= 0.01f) continue;
        int a = (int)(label.alpha * 100);
        float vx = bassVib > 0.1f ? ofRandom(-bassVib, bassVib) : 0;
        float vy = bassVib > 0.1f ? ofRandom(-bassVib, bassVib) : 0;
        float tx = label.pos.x + 10 + vx, ty = label.pos.y - 5 + vy;
        ofFill();
        ofSetColor(200, 220, 255, a);
        ofDrawRectangle(tx - 6, ty - 10, 4, 4);
        ofDrawBitmapString(label.text, tx, ty);
    }
    ofPopStyle();
}

// ── Update ────────────────────────────────────────────────────────────────────
void ofApp::update() {
    std::vector<float> buf; float localRms;
    { std::lock_guard<std::mutex> lock(audioMutex); buf = audioBuffer; localRms = rawRms; }

    float w  = ofGetWidth(), h = ofGetHeight();
    float t  = ofGetElapsedTimef();
    float dt = std::max((float)ofGetLastFrameTime(), 0.001f);

    processOscMessages();
    applyMidiParams(dt);
    if (frozen) return;

    fft->setSignal(buf);
    float* amp = fft->getAmplitude(); int numBins = fft->getBinSize();

    auto bandEnergy = [&](int lo, int hi) {
        hi = std::min(hi, numBins - 1);
        float s = 0; for (int i = lo; i <= hi; i++) s += amp[i];
        return s / (hi - lo + 1);
    };
    float rawBass = bandEnergy(BASS_LO, BASS_HI);
    float rawMid  = bandEnergy(MID_LO,  MID_HI);
    float rawHigh = bandEnergy(HIGH_LO, HIGH_HI);

    float sumAF = 0, sumA = 0;
    int cHi = std::min((int)(8000.0f / BIN_FREQ), numBins - 1);
    for (int i = 0; i <= cHi; i++) { sumAF += i * BIN_FREQ * amp[i]; sumA += amp[i]; }
    float rawCentroid = (sumA > 1e-6f) ? ofClamp(sumAF / sumA / 8000.0f, 0.0f, 1.0f) : 0.0f;

    float rawFlux = 0;
    for (int i = 0; i < numBins; i++) rawFlux += std::max(0.0f, amp[i] - prevAmplitude[i]);
    std::copy(amp, amp + numBins, prevAmplitude.begin());

    sBass     = asymSmooth(sBass,     rawBass,     0.8f, 0.95f);
    sMid      = asymSmooth(sMid,      rawMid,      0.8f, 0.95f);
    sHigh     = asymSmooth(sHigh,     rawHigh,     0.8f, 0.95f);
    sRms      = asymSmooth(sRms,      localRms,    0.8f, 0.95f);
    sCentroid = asymSmooth(sCentroid, rawCentroid, 0.5f, 0.98f);
    sFlux     = asymSmooth(sFlux,     rawFlux,     0.9f, 0.9f);

    float effBass = sBass * mBassSens;
    float effMid  = sMid  * mMidSens;
    float effHigh = sHigh * mHighSens;

    onset = (sFlux > FLUX_ONSET_THRESH && sRms > 0.01f);
    onsetRising = onset && !prevOnset;
    prevOnset   = onset;

    // Override with OSC FFT values if active
    if (oscFftActive) {
        sBass     = oscBass;
        sMid      = oscMid;
        sHigh     = oscHigh;
        sRms      = oscRms;
        sCentroid = oscCentroid;
        sFlux     = oscFlux;
        onset     = oscOnset;
        effBass   = sBass * mBassSens;
        effMid    = sMid  * mMidSens;
        effHigh   = sHigh * mHighSens;
        onsetRising = onset && !prevOnset;
        prevOnset   = onset;
    }

    // エフェクトパラメータ更新
    updateEffectParams(dt);

    // タイポグラフィ
    updateTypography(dt, w, h);

    // ── 流体への力注入 ──
    if (fluidRunning) {
        injectTypography(w, h);

        if (effBass > 0.05f) {
            float r = onset ? 40.0f : 20.0f, tp = onset ? 20.0f : 10.0f;
            fluid.addTemporalForce(ofVec2f(w*.5f+ofRandom(-50,50), h*.6f),
                                   ofVec2f(0,-effBass*80),
                                   paletteColor(ofRandom(0.0f, 0.3f)), r, tp, 1.0f);
        }
        if (effMid > 0.02f) {
            bool L = sinf(t*3)>0;
            fluid.addTemporalForce(ofVec2f(L?w*.2f:w*.8f, h*(1.0f-sCentroid*.8f)),
                                   ofVec2f(L?effMid*60:-effMid*60, 0),
                                   paletteColor(L?0.5f:0.35f), 15.0f, 5.0f, 0.8f);
        }
        if (effHigh > 0.01f && sFlux > 0.1f) {
            for (int i = 0; i < 3; i++)
                fluid.addTemporalForce(ofVec2f(ofRandom(w), ofRandom(h)),
                                       ofVec2f(ofRandom(-1,1)*effHigh*30, ofRandom(-1,1)*effHigh*30),
                                       paletteColor(fmodf(t*.3f+i*.1f,1)), 8.0f, 2.0f, 0.5f);
        }
        if (onset) {
            float bh = fmodf(t*.2f, 1.0f);
            for (int i = 0; i < 16; i++) {
                float a = i*TWO_PI/16 + ofRandom(-0.2f,0.2f), sp = sFlux*50*ofRandom(.6f,1.4f);
                fluid.addTemporalForce(ofVec2f(w*.5f,h*.5f),
                                       ofVec2f(cosf(a)*sp, sinf(a)*sp),
                                       paletteColor(fmodf(bh+i/16.0f,1)), 40.0f, 15.0f, 1.0f);
            }
        }
        fluid.update();
    }

    updateWireframe(dt, t);
    updatePlexus(dt, t, w, h);

    if (showCoordText && plexusVisible) {
        int np = (int)plexusParticles.size();
        plexusConnCounts.assign(np, 0);
        for (int i = 0; i < np; i++)
            for (int j = i+1; j < np; j++)
                if (plexusParticles[i].pos.distance(plexusParticles[j].pos) < mPlexusThresh) {
                    plexusConnCounts[i]++; plexusConnCounts[j]++;
                }
    }

    if (onsetRising) {
        spawnPolygons((int)(ofRandom(5,16) * mGlitchFreq), true);
        flashAlpha = 30.0f;
    }
    bassSpawnCooldown -= dt;
    if (effBass > 0.08f && bassSpawnCooldown <= 0) {
        spawnPolygons((int)ofRandom(1,4), false, 150.0f*(1+effBass));
        bassSpawnCooldown = mGlitchFreq > 0.01f ? 0.15f / mGlitchFreq : 10.0f;
    }
    highSpawnCooldown -= dt;
    if (effHigh > 0.05f && highSpawnCooldown <= 0) {
        int n = (int)ofRandom(0,3); if (n>0) spawnPolygons(n, false, 50.0f);
        highSpawnCooldown = mGlitchFreq > 0.01f ? 0.08f / mGlitchFreq : 10.0f;
    }
    flashAlpha = std::max(0.0f, flashAlpha - 10.0f);
    for (auto& p : polygons) {
        p.lifetime -= dt; p.rotation += p.rotSpeed * dt;
        p.alpha = p.baseAlpha * (p.lifetime / p.maxLifetime);
    }
    polygons.erase(std::remove_if(polygons.begin(), polygons.end(),
                   [](const GlitchPolygon& p){ return p.lifetime <= 0; }), polygons.end());

    updateCoordLabels(dt);
}

// ── 3D Wireframe Setup ────────────────────────────────────────────────────────
void ofApp::setupWireframe() {
    ofIcoSpherePrimitive sphere; sphere.setRadius(150.0f); sphere.setResolution(2);
    sphereMesh = sphere.getMesh();
    origVerts = sphereMesh.getVertices();
    origNorms.resize(origVerts.size());
    for (size_t i = 0; i < origVerts.size(); i++)
        origNorms[i] = glm::normalize(origVerts[i]);
}

void ofApp::updateWireframe(float dt, float t) {
    rotBoost = ofLerp(rotBoost, 1.0f, 0.05f);
    if (onsetRising) rotBoost = 3.0f;
    rotY += mRotSpeed * rotBoost * dt;
    rotX  = ofLerp(rotX, sCentroid * 30.0f, 0.05f);

    int nv = (int)origVerts.size();
    meshDisplacements.resize(nv);
    for (int i = 0; i < nv; i++) {
        const glm::vec3 o = origVerts[i], n = origNorms[i];
        float d = (ofNoise(o.x*.01f, o.y*.01f, t*.5f) * 30.0f
                + sBass * 80.0f + sinf(i * 0.3f + t) * sMid * 50.0f
                + (onset ? ofRandom(-1,1) * sHigh * 20.0f : 0)) * mDisplacement;
        meshDisplacements[i] = fabsf(d);
        sphereMesh.setVertex(i, o + n * d);
    }
}

void ofApp::drawWireframe() {
    float w = ofGetWidth(), h = ofGetHeight();
    float radX = rotX * DEG_TO_RAD, radY = rotY * DEG_TO_RAD;
    float cosX = cosf(radX), sinX = sinf(radX), cosY = cosf(radY), sinY = sinf(radY);

    ofEnableDepthTest();
    ofPushMatrix();
    ofTranslate(w*.5f, h*.5f, 0);
    ofRotateDeg(rotX, 1, 0, 0); ofRotateDeg(rotY, 0, 1, 0);

    float gs = 1.02f + sBass * 0.03f;
    ofPushStyle(); ofSetColor(255,255,255,40); ofSetLineWidth(1.0f);
    ofPushMatrix(); ofScale(gs); sphereMesh.drawWireframe(); ofPopMatrix();
    ofPopStyle();

    ofPushStyle(); ofSetLineWidth(1.5f);
    if (onset) ofSetColor(((int)(ofGetElapsedTimef()*30)%2)==0 ? ofColor::cyan : ofColor::magenta, 180);
    else ofSetColor(255,255,255,180);
    sphereMesh.drawWireframe();
    ofPopStyle();

    ofPopMatrix(); ofDisableDepthTest();

    meshScreenPos.clear();
    meshScreenPos.reserve(sphereMesh.getNumVertices());
    for (const auto& v : sphereMesh.getVertices()) {
        float x1 = v.x*cosY + v.z*sinY, y1 = v.y, z1 = -v.x*sinY + v.z*cosY;
        meshScreenPos.push_back(ofVec2f(w*.5f + x1, h*.5f + y1*cosX - z1*sinX));
    }

    ofPushStyle();
    ofSetColor(255,255,255,(int)ofClamp(sBass*600,30,200));
    for (const auto& sp : meshScreenPos) ofDrawCircle(sp.x, sp.y, 2.0f);
    ofPopStyle();

    ofPushStyle(); ofSetLineWidth(1.0f);
    const float mpt = mPlexusThresh;
    for (const auto& mp : meshScreenPos) {
        for (const auto& pp : plexusParticles) {
            float d = mp.distance(ofVec2f(pp.pos.x, pp.pos.y));
            if (d < mpt) {
                float a = (1.0f - d/mpt) * 100.0f;
                if (onset) a = std::min(a * 2.0f, 255.0f);
                ofSetColor(200,220,255,(int)a);
                ofDrawLine(mp.x, mp.y, pp.pos.x, pp.pos.y);
            }
        }
    }
    ofPopStyle();
}

// ── Plexus ────────────────────────────────────────────────────────────────────
void ofApp::setupPlexus() {
    float w = ofGetWidth(), h = ofGetHeight();
    plexusParticles.resize(120);
    for (auto& p : plexusParticles) {
        p.pos = ofVec3f(ofRandom(w), ofRandom(h), ofRandom(-200, 200));
        p.vel = ofVec3f(0); p.radius = ofRandom(1.0f, 3.0f); p.alpha = ofRandom(100, 200);
    }
}

void ofApp::updatePlexus(float dt, float t, float w, float h) {
    for (int i = 0; i < (int)plexusParticles.size(); i++) {
        auto& p = plexusParticles[i];
        float nx = ofNoise(p.pos.x*.005f, t*.3f, i*.1f)*2-1;
        float ny = ofNoise(p.pos.y*.005f, t*.3f+50, i*.1f)*2-1;
        float nz = ofNoise(p.pos.z*.005f, t*.3f+100, i*.1f)*2-1;
        p.vel += ofVec3f(nx, ny, nz) * 0.5f * mPlexusSpeed;
        if (sBass > 0.05f) {
            ofVec3f fc = p.pos - ofVec3f(w*.5f, h*.5f, 0);
            if (fc.length() > 1.0f) p.vel += fc.getNormalized() * sBass * 2.0f;
        }
        p.vel.y += sMid * 1.5f * dt;
        if (onsetRising) p.vel += ofVec3f(ofRandom(-5,5), ofRandom(-5,5), ofRandom(-2,2));
        p.vel *= 0.95f; p.pos += p.vel * dt;
        if (p.pos.x < 0) p.pos.x += w; else if (p.pos.x > w) p.pos.x -= w;
        if (p.pos.y < 0) p.pos.y += h; else if (p.pos.y > h) p.pos.y -= h;
        if (p.pos.z < -200) p.pos.z = 200; else if (p.pos.z > 200) p.pos.z = -200;
    }
}

void ofApp::drawPlexusLayer(float w, float h) {
    auto lineCol = [&](float a) -> ofColor {
        int r, g, b;
        if (sCentroid < 0.5f) {
            float tt = sCentroid*2.0f; r=(int)ofLerp(150,255,tt); g=(int)ofLerp(180,255,tt); b=255;
        } else {
            float tt = (sCentroid-0.5f)*2.0f; r=255; g=(int)ofLerp(255,180,tt); b=(int)ofLerp(255,100,tt);
        }
        return ofColor(r,g,b,(int)ofClamp(a,0,255));
    };
    ofPushStyle(); ofSetLineWidth(1.0f);
    const float thresh = mPlexusThresh;
    std::vector<int> cc(plexusParticles.size(), 0);
    int total = 0;
    for (int i = 0; i < (int)plexusParticles.size() && total < 300; i++) {
        if (cc[i] >= 5) continue;
        for (int j = i+1; j < (int)plexusParticles.size() && total < 300; j++) {
            if (cc[j] >= 5) continue;
            float d = plexusParticles[i].pos.distance(plexusParticles[j].pos);
            if (d < thresh) {
                float a = (1.0f-d/thresh)*80.0f;
                float depth = ((plexusParticles[i].pos.z+plexusParticles[j].pos.z)*.5f < 0) ? .5f : 1.0f;
                ofSetColor(lineCol(a*depth));
                ofDrawLine(plexusParticles[i].pos.x, plexusParticles[i].pos.y,
                           plexusParticles[j].pos.x, plexusParticles[j].pos.y);
                cc[i]++; cc[j]++; total++;
            }
        }
    }
    for (const auto& p : plexusParticles) {
        float depth = p.pos.z < 0 ? 0.5f : 1.0f;
        ofSetColor(lineCol(p.alpha*depth));
        ofDrawCircle(p.pos.x, p.pos.y, p.radius*(p.pos.z < 0 ? 0.7f : 1.0f));
    }
    ofPopStyle();
}

// ── Glitch Spawn ──────────────────────────────────────────────────────────────
void ofApp::spawnPolygons(int count, bool isOnset, float maxRadius) {
    float w = ofGetWidth(), h = ofGetHeight();
    for (int i = 0; i < count; i++) {
        if ((int)polygons.size() >= 100) polygons.erase(polygons.begin());
        GlitchPolygon p;
        p.position = isOnset ? ofVec2f(w*.5f+ofRandom(-200,200), h*.5f+ofRandom(-200,200))
                             : ofVec2f(ofRandom(w), ofRandom(h));
        int nv = (int)ofRandom(3,7);
        for (int j = 0; j < nv; j++) {
            float a = j*TWO_PI/nv + ofRandom(-30,30)*DEG_TO_RAD;
            float r = ofRandom(std::max(20.0f, maxRadius*.2f), maxRadius);
            p.vertices.push_back(ofVec2f(cosf(a)*r, sinf(a)*r));
        }
        p.maxLifetime = p.lifetime = ofRandom(0.05f, 0.4f);
        p.rotation = ofRandom(360); p.rotSpeed = ofRandom(-180,180);
        p.scale = ofRandom(0.5f, 2.0f); p.filled = ofRandom(1.0f) < 0.3f;
        int g = (int)ofRandom(200,256);
        p.color = ofColor(g,g,g); p.strokeColor = ofColor(255);
        p.baseAlpha = p.alpha = ofRandom(100,256);
        p.chromatic = isOnset; p.chromaticOffset = isOnset ? ofRandom(3,10) : 0.0f;
        polygons.push_back(p);
    }
}

void ofApp::drawPolygon(const GlitchPolygon& p, float offsetX,
                        const ofColor& col, float alphaScale) {
    ofPushMatrix();
    ofTranslate(p.position.x + offsetX, p.position.y);
    ofRotateDeg(p.rotation); ofScale(p.scale);
    int a = (int)ofClamp(p.alpha * alphaScale, 0, 255);
    if (p.filled) { ofFill(); ofSetColor(col.r, col.g, col.b, a); }
    else { ofNoFill(); ofSetLineWidth(ofRandom(1,4)); ofSetColor(p.strokeColor.r, p.strokeColor.g, p.strokeColor.b, a); }
    ofBeginShape();
    for (const auto& v : p.vertices) ofVertex(v.x, v.y);
    ofEndShape(true);
    ofPopMatrix();
    float rad = p.rotation * DEG_TO_RAD, cosR = cosf(rad), sinR = sinf(rad);
    for (const auto& v : p.vertices) {
        float sx = v.x*p.scale, sy = v.y*p.scale;
        ofDrawLine(0, 0, (sx*cosR-sy*sinR)+p.position.x+offsetX, (sx*sinR+sy*cosR)+p.position.y);
    }
}

// ── Draw ──────────────────────────────────────────────────────────────────────
void ofApp::draw() {
    float w = ofGetWidth(), h = ofGetHeight();
    float* amp = fft->getAmplitude(); int numBins = fft->getBinSize();
    int showBins = std::min(numBins, HIGH_HI+1); float barW = w/showBins, maxBarH = 100.0f;

    // 1. 流体 + スペクトラム → renderFbo
    renderFbo.begin(); ofClear(0,0,0,255);
    fluid.draw(0, 0, w, h);
    ofPushStyle();
    for (int i = 0; i < showBins; i++) {
        float bh = ofClamp(amp[i]*maxBarH*10, 0, maxBarH);
        if      (i<=BASS_HI) ofSetColor(200,50,50,160);
        else if (i<=MID_HI)  ofSetColor(50,200,50,160);
        else                 ofSetColor(50,50,200,160);
        ofDrawRectangle(i*barW, h-bh, barW, bh);
    }
    ofSetColor(255,255,255,200);
    ofDrawLine((sCentroid*8000.0f/BIN_FREQ)*barW, h-maxBarH, (sCentroid*8000.0f/BIN_FREQ)*barW, h);
    ofPopStyle();
    renderFbo.end();

    // 2. 全レイヤーを sceneFbo に合成
    sceneFbo.begin(); ofClear(0,0,0,255);

    if (bloomEnabled) {
        float bloomThresh = onset ? std::max(0.0f, mBloomThreshold - 0.2f) : mBloomThreshold;
        brightFbo.begin(); ofClear(0,0,0,255);
        brightnessShader.begin();
        brightnessShader.setUniformTexture("tex", renderFbo.getTexture(), 0);
        brightnessShader.setUniform1f("threshold", bloomThresh);
        renderFbo.draw(0,0,w,h); brightnessShader.end(); brightFbo.end();

        float ba = 1.0f + sBass*3.0f;
        blurHFbo.begin(); ofClear(0,0,0,255);
        blurShader.begin();
        blurShader.setUniformTexture("tex", brightFbo.getTexture(), 0);
        blurShader.setUniform2f("direction",1,0); blurShader.setUniform1f("blurAmount",ba);
        brightFbo.draw(0,0,w,h); blurShader.end(); blurHFbo.end();

        blurVFbo.begin(); ofClear(0,0,0,255);
        blurShader.begin();
        blurShader.setUniformTexture("tex", blurHFbo.getTexture(), 0);
        blurShader.setUniform2f("direction",0,1); blurShader.setUniform1f("blurAmount",ba);
        blurHFbo.draw(0,0,w,h); blurShader.end(); blurVFbo.end();

        bloomShader.begin();
        bloomShader.setUniformTexture("baseTex", renderFbo.getTexture(), 0);
        bloomShader.setUniformTexture("bloomTex", blurVFbo.getTexture(), 1);
        bloomShader.setUniform1f("bloomIntensity", mBloomIntensity + sRms*2.0f);
        renderFbo.draw(0,0,w,h); bloomShader.end();
    } else {
        renderFbo.draw(0,0,w,h);
    }

    if (plexusVisible) drawPlexusLayer(w, h);
    drawWireframe();

    if (flashAlpha > 0) {
        ofPushStyle(); ofSetColor(255,255,255,(int)flashAlpha);
        ofDrawRectangle(0,0,w,h); ofPopStyle();
    }

    ofPushStyle();
    for (const auto& p : polygons) {
        if (p.chromatic) {
            drawPolygon(p, p.chromaticOffset, ofColor(255,0,0), 0.5f);
            drawPolygon(p, -p.chromaticOffset, ofColor(0,255,255), 0.5f);
        }
        drawPolygon(p, 0.0f, p.color, 1.0f);
    }
    ofPopStyle();

    drawCoordLabels();

    sceneFbo.end();

    // 3. エフェクトチェーン（ピンポン FBO）→ 画面
    applyEffectChain(w, h);

    // 3.5 Syphon 出力（エフェクト適用後、UI描画前）
    // applyEffectChain が最終結果を画面に描画済み。
    // 最終 FBO のテクスチャを publish する。
    // applyEffectChain 内で src が最終結果を指しているが、
    // ここでは sceneFbo を使う（エフェクトなしの場合）か effectFboA/B。
    // 簡易的に publishScreen を使う（画面全体をキャプチャ、UI描画前）
    syphonServer.publishScreen();

    // 4. マスター明るさ
    if (mMasterBright < 0.99f) {
        ofPushStyle();
        ofSetColor(0, 0, 0, (int)((1.0f - mMasterBright) * 510));
        ofDrawRectangle(0,0,w,h); ofPopStyle();
    } else if (mMasterBright > 1.01f) {
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofSetColor(255, 255, 255, (int)((mMasterBright - 1.0f) * 510));
        ofDrawRectangle(0,0,w,h);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofPopStyle();
    }

    // 5. ヘルプオーバーレイ
    if (showHelp) {
        ofPushStyle();
        ofSetColor(0, 0, 0, 180);
        ofDrawRectangle(w*0.3f, h*0.15f, w*0.4f, h*0.7f);
        ofSetColor(255);
        int y = (int)(h*0.2f);
        int x = (int)(w*0.33f);
        ofDrawBitmapString("=== KEY BINDINGS ===", x, y); y+=20;
        ofDrawBitmapString("F  Fullscreen", x, y); y+=16;
        ofDrawBitmapString("D  Debug display", x, y); y+=16;
        ofDrawBitmapString("T  Coord text", x, y); y+=16;
        ofDrawBitmapString("H  This help", x, y); y+=16;
        ofDrawBitmapString("R  Fluid pause", x, y); y+=16;
        ofDrawBitmapString("V  Typography", x, y); y+=16;
        ofDrawBitmapString("A  Switch Audio Device", x, y); y+=24;
        ofDrawBitmapString("--- EFFECTS ---", x, y); y+=18;
        for (int i = 0; i < FX_COUNT; i++) {
            char c = (char)fx[i].key;
            string label = string(1, (c>='a'&&c<='z') ? (char)toupper(c) : c)
                         + "  " + fx[i].name + ": " + (fx[i].enabled?"ON":"OFF");
            ofDrawBitmapString(label, x, y); y+=16;
        }
        y+=8;
        ofDrawBitmapString("--- PRESETS ---", x, y); y+=18;
        ofDrawBitmapString("Cmd+S     Quick Save", x, y); y+=16;
        ofDrawBitmapString("Cmd+L     Quick Load", x, y); y+=16;
        ofDrawBitmapString("Cmd+1~8   Save Slot", x, y); y+=16;
        ofDrawBitmapString("Shift+1~8 Load Slot", x, y); y+=16;
        ofPopStyle();
    }

    // 6. デバッグテキスト
    if (showDebug) {
        int y = 20;
        ofDrawBitmapStringHighlight("fps:"+ofToString((int)ofGetFrameRate()), 10,y); y+=16;
        ofDrawBitmapStringHighlight("bass:"+ofToString(sBass,4)+" mid:"+ofToString(sMid,4)
            +" high:"+ofToString(sHigh,4), 10,y); y+=16;
        ofDrawBitmapStringHighlight("rms:"+ofToString(sRms,4)+" cent:"+ofToString(sCentroid,4)
            +" flux:"+ofToString(sFlux,4)+" onset:"+(onset?"Y":"-"), 10,y); y+=20;

        // エフェクト状態を1行で
        string fxLine = "FX:";
        for (int i = 0; i < FX_COUNT; i++) {
            char c = (char)fx[i].key;
            fxLine += string(" [") + (char)toupper(c) + "]" + fx[i].name.substr(0,3)
                    + ":" + (fx[i].enabled?"ON":"--");
        }
        fxLine += " [V]typ:" + string(typoEnabled?"ON":"--");
        ofDrawBitmapStringHighlight(fxLine, 10, y); y+=16;

        ofDrawBitmapStringHighlight("bloom:"+(bloomEnabled?string("ON"):"OFF")
            +" plexus:"+(plexusVisible?"ON":"OFF")
            +" freeze:"+(frozen?"ON":"OFF")
            +" [H]elp", 10, y); y+=16;

        string audioName = (currentAudioDevice < (int)inputDevices.size())
            ? inputDevices[currentAudioDevice].name : "default";
        ofDrawBitmapStringHighlight("[Syphon:VJCosmos] [Audio:" + audioName + "]"
            + (lastPresetName.empty() ? "" : " [Preset:" + lastPresetName + "]"), 10, y);
    }

    // 7. OSC デバッグオーバーレイ
    drawOscDebug();
}

// ── OSC Debug Overlay ─────────────────────────────────────────────────────────
void ofApp::drawOscDebug() {
    if (!showOscDebug) return;

    float w = ofGetWidth(), h = ofGetHeight();
    float now = ofGetElapsedTimef();
    ofPushStyle();
    ofSetColor(0, 0, 0, 180);
    ofDrawRectangle(0, 0, w, h);

    int x = 20, y = 30;

    // Color helpers based on OscParamLog state
    auto paramColor = [&](const string& key) -> ofColor {
        auto it = oscParamLog.find(key);
        if (it == oscParamLog.end() || !it->second.everReceived)
            return ofColor(255, 80, 80);   // red: never received
        if (now - it->second.receivedAt < 1.0f)
            return ofColor(0, 255, 100);   // green: recently updated
        return ofColor(255);               // white: received but stale
    };
    auto recvStr = [&](const string& key) -> string {
        auto it = oscParamLog.find(key);
        return (it != oscParamLog.end() && it->second.everReceived)
            ? ofToString(it->second.value, 4) : "---";
    };
    auto mapStr = [&](const string& key) -> string {
        auto it = oscParams.find(key);
        return it != oscParams.end() ? ofToString(it->second, 4) : "---";
    };

    // ── Section A: Summary ──
    float ago = now - lastOscTime;
    ofSetColor(255, 255, 0);
    ofDrawBitmapString("=== OSC DEBUG [O] close ===", x, y); y += 18;
    ofSetColor(255);
    string summary = "total:" + ofToString(oscMsgCount)
        + " | fft:" + ofToString(oscFftMsgCount)
        + " | param:" + ofToString(oscParamMsgCount)
        + " | last: " + lastOscAddr + " (" + ofToString(ago, 1) + "s ago)";
    ofDrawBitmapString(summary, x, y); y += 22;

    // ── Section B: FFT values ──
    ofSetColor(0, 220, 220);
    ofDrawBitmapString("--- FFT (active:" + string(oscFftActive ? "YES" : "NO")
        + " timeout:" + ofToString(oscFftTimeout, 1) + ") ---", x, y); y += 16;
    struct FftRow { const char* name; float oscVal; float usedVal; };
    FftRow fftRows[] = {
        {"bass",     oscBass,     sBass},
        {"mid",      oscMid,      sMid},
        {"high",     oscHigh,     sHigh},
        {"rms",      oscRms,      sRms},
        {"centroid", oscCentroid, sCentroid},
        {"flux",     oscFlux,     sFlux},
    };
    for (auto& r : fftRows) {
        ofSetColor(oscFftActive ? ofColor(0,255,100) : ofColor(255));
        char buf[120];
        snprintf(buf, sizeof(buf), "%-12s OSC:%-8.4f | used:%-8.4f | src:%s",
                 r.name, r.oscVal, r.usedVal, oscFftActive ? "OSC" : "LOCAL");
        ofDrawBitmapString(buf, x, y); y += 14;
    }
    {
        ofSetColor(oscFftActive ? ofColor(0,255,100) : ofColor(255));
        char buf[80];
        snprintf(buf, sizeof(buf), "%-12s OSC:%-8s | used:%-8s | src:%s",
                 "onset", oscOnset?"true":"false", onset?"true":"false", oscFftActive?"OSC":"LOCAL");
        ofDrawBitmapString(buf, x, y); y += 20;
    }

    // ── Section C: Continuous params ──
    ofSetColor(0, 220, 220);
    ofDrawBitmapString("--- CONTINUOUS PARAMS ---", x, y); y += 16;
    struct ParamRow { const char* name; const char* key; float varVal; };
    ParamRow pRows[] = {
        {"dissipation",     "dissipation",     mDissipation},
        {"velDissipation",  "velDissipation",  mVelDissipation},
        {"gravity",         "gravity",         mGravity},
        {"bloomIntensity",  "bloomIntensity",  mBloomIntensity},
        {"bloomThreshold",  "bloomThreshold",  mBloomThreshold},
        {"bassSens",        "bassSens",        mBassSens},
        {"midSens",         "midSens",         mMidSens},
        {"highSens",        "highSens",        mHighSens},
        {"rotSpeed",        "rotSpeed",        mRotSpeed},
        {"displacement",    "displacement",    mDisplacement},
        {"glitchFreq",      "glitchFreq",      mGlitchFreq},
        {"fluidSaturation", "fluidSaturation", mFluidSaturation},
        {"plexusSpeed",     "plexusSpeed",     mPlexusSpeed},
        {"masterBrightness","masterBrightness", mMasterBright},
    };
    for (auto& r : pRows) {
        ofSetColor(paramColor(r.key));
        char buf[140];
        snprintf(buf, sizeof(buf), "%-18s recv:%-8s | map:%-8s | var:%-10.4f",
                 r.name, recvStr(r.key).c_str(), mapStr(r.key).c_str(), r.varVal);
        ofDrawBitmapString(buf, x, y); y += 14;
    }
    y += 6;

    // ── Section D: Effect toggles ──
    ofSetColor(0, 220, 220);
    ofDrawBitmapString("--- EFFECT TOGGLES ---", x, y); y += 16;
    struct TogRow { const char* name; const char* key; int fxIdx; };
    TogRow tRows[] = {
        {"fxKaleido", "fxKaleido", FX_KALEIDO},
        {"fxCrt",     "fxCrt",     FX_CRT},
        {"fxWave",    "fxWave",    FX_WAVE},
        {"fxGlitch",  "fxGlitch",  FX_GLITCH},
        {"fxEdge",    "fxEdge",    FX_EDGE},
        {"fxMono",    "fxMono",    FX_MONO},
        {"fxMirror",  "fxMirror",  FX_MIRROR},
    };
    for (auto& r : tRows) {
        ofSetColor(paramColor(r.key));
        char buf[120];
        snprintf(buf, sizeof(buf), "%-12s recv:%-8s | map:%-8s | enabled:%-5s",
                 r.name, recvStr(r.key).c_str(), mapStr(r.key).c_str(),
                 fx[r.fxIdx].enabled ? "true" : "false");
        ofDrawBitmapString(buf, x, y); y += 14;
    }
    y += 6;

    // ── Effect params ──
    ofSetColor(0, 220, 220);
    ofDrawBitmapString("--- EFFECT PARAMS ---", x, y); y += 16;
    ParamRow epRows[] = {
        {"waveAmplitude",  "waveAmplitude",  waveAmplitude},
        {"waveSpeed",      "waveSpeed",      waveSpeed},
        {"edgeStrength",   "edgeStrength",   edgeStrength},
        {"edgeMix",        "edgeMix",        edgeMix},
        {"monoMode",       "monoMode",       monoMode},
        {"mirrorMode",     "mirrorMode",     mirrorMode},
        {"colorPalette",   "colorPalette",   (float)colorPalette},
    };
    for (auto& r : epRows) {
        ofSetColor(paramColor(r.key));
        char buf[140];
        snprintf(buf, sizeof(buf), "%-18s recv:%-8s | map:%-8s | var:%-10.4f",
                 r.name, recvStr(r.key).c_str(), mapStr(r.key).c_str(), r.varVal);
        ofDrawBitmapString(buf, x, y); y += 14;
    }

    ofPopStyle();
}

// ── Preset Save ───────────────────────────────────────────────────────────────
void ofApp::savePreset(const string& name) {
    ofJson j;
    j["version"] = 1;
    j["name"] = name;

    j["fluid"]["dissipation"] = mDissipation;
    j["fluid"]["velocityDissipation"] = mVelDissipation;
    j["fluid"]["gravity"] = mGravity;

    j["bloom"]["enabled"] = bloomEnabled;
    j["bloom"]["intensity"] = mBloomIntensity;
    j["bloom"]["threshold"] = mBloomThreshold;

    j["audio"]["bassSens"] = mBassSens;
    j["audio"]["midSens"] = mMidSens;
    j["audio"]["highSens"] = mHighSens;

    j["effects"]["kaleidoscope"]["enabled"] = fx[FX_KALEIDO].enabled;
    j["effects"]["kaleidoscope"]["segments"] = kaleidoSegments;
    j["effects"]["crt"]["enabled"] = fx[FX_CRT].enabled;
    j["effects"]["wave"]["enabled"] = fx[FX_WAVE].enabled;
    j["effects"]["wave"]["amplitude"] = waveAmplitude;
    j["effects"]["wave"]["frequency"] = waveFrequency;
    j["effects"]["wave"]["speed"] = waveSpeed;
    j["effects"]["glitch"]["enabled"] = fx[FX_GLITCH].enabled;
    j["effects"]["glitch"]["intensity"] = glitchIntensity;
    j["effects"]["glitch"]["blockSize"] = glitchBlockSize;
    j["effects"]["edge"]["enabled"] = fx[FX_EDGE].enabled;
    j["effects"]["edge"]["strength"] = edgeStrength;
    j["effects"]["edge"]["mix"] = edgeMix;
    j["effects"]["mono"]["enabled"] = fx[FX_MONO].enabled;
    j["effects"]["mono"]["mode"] = monoMode;
    j["effects"]["mono"]["intensity"] = monoIntensity;
    j["effects"]["mirror"]["enabled"] = fx[FX_MIRROR].enabled;
    j["effects"]["mirror"]["mode"] = mirrorMode;

    j["visuals"]["plexusVisible"] = plexusVisible;
    j["visuals"]["colorPalette"] = (int)colorPalette;
    j["visuals"]["meshGeometry"] = (int)meshGeometry;
    j["visuals"]["rotSpeed"] = mRotSpeed;
    j["visuals"]["displacement"] = mDisplacement;
    j["visuals"]["glitchFreq"] = mGlitchFreq;
    j["visuals"]["fluidSaturation"] = mFluidSaturation;
    j["visuals"]["plexusSpeed"] = mPlexusSpeed;
    j["visuals"]["masterBrightness"] = mMasterBright;
    j["visuals"]["plexusThresh"] = mPlexusThresh;
    j["visuals"]["typoEnabled"] = typoEnabled;

    string path = "presets/" + name + ".json";
    ofSaveJson(path, j);
    lastPresetName = name;
    ofLogNotice("PRESET") << "saved: " << path;
}

// ── Preset Load ───────────────────────────────────────────────────────────────
void ofApp::loadPreset(const string& name) {
    string path = "presets/" + name + ".json";
    ofFile file(path);
    if (!file.exists()) {
        ofLogWarning("PRESET") << "not found: " << path;
        return;
    }

    ofJson j = ofLoadJson(path);

    if (j.contains("fluid")) {
        mDissipation    = j["fluid"].value("dissipation", mDissipation);
        mVelDissipation = j["fluid"].value("velocityDissipation", mVelDissipation);
        mGravity        = j["fluid"].value("gravity", mGravity);
    }
    if (j.contains("bloom")) {
        bloomEnabled    = j["bloom"].value("enabled", bloomEnabled);
        mBloomIntensity = j["bloom"].value("intensity", mBloomIntensity);
        mBloomThreshold = j["bloom"].value("threshold", mBloomThreshold);
    }
    if (j.contains("audio")) {
        mBassSens = j["audio"].value("bassSens", mBassSens);
        mMidSens  = j["audio"].value("midSens", mMidSens);
        mHighSens = j["audio"].value("highSens", mHighSens);
    }
    if (j.contains("effects")) {
        auto& e = j["effects"];
        if (e.contains("kaleidoscope")) {
            fx[FX_KALEIDO].enabled = e["kaleidoscope"].value("enabled", false);
            kaleidoSegments = e["kaleidoscope"].value("segments", 6);
        }
        if (e.contains("crt"))
            fx[FX_CRT].enabled = e["crt"].value("enabled", false);
        if (e.contains("wave")) {
            fx[FX_WAVE].enabled = e["wave"].value("enabled", false);
            waveAmplitude = e["wave"].value("amplitude", 10.0f);
            waveFrequency = e["wave"].value("frequency", 0.05f);
            waveSpeed     = e["wave"].value("speed", 2.0f);
        }
        if (e.contains("glitch")) {
            fx[FX_GLITCH].enabled = e["glitch"].value("enabled", false);
            glitchIntensity = e["glitch"].value("intensity", 0.3f);
            glitchBlockSize = e["glitch"].value("blockSize", 30.0f);
        }
        if (e.contains("edge")) {
            fx[FX_EDGE].enabled = e["edge"].value("enabled", false);
            edgeStrength = e["edge"].value("strength", 1.0f);
            edgeMix      = e["edge"].value("mix", 0.3f);
        }
        if (e.contains("mono")) {
            fx[FX_MONO].enabled = e["mono"].value("enabled", false);
            monoMode      = e["mono"].value("mode", 0.0f);
            monoIntensity = e["mono"].value("intensity", 1.0f);
        }
        if (e.contains("mirror")) {
            fx[FX_MIRROR].enabled = e["mirror"].value("enabled", false);
            mirrorMode = e["mirror"].value("mode", 0.0f);
        }
    }
    if (j.contains("visuals")) {
        auto& v = j["visuals"];
        plexusVisible    = v.value("plexusVisible", true);
        colorPalette     = (ColorPalette)v.value("colorPalette", (int)PALETTE_RAINBOW);
        int geom         = v.value("meshGeometry", (int)GEOM_ICOSPHERE);
        if (geom != (int)meshGeometry) {
            meshGeometry = (MeshGeometry)geom;
            rebuildMesh();
        }
        mRotSpeed        = v.value("rotSpeed", 15.0f);
        mDisplacement    = v.value("displacement", 1.0f);
        mGlitchFreq      = v.value("glitchFreq", 1.0f);
        mFluidSaturation = v.value("fluidSaturation", 1.0f);
        mPlexusSpeed     = v.value("plexusSpeed", 1.0f);
        mMasterBright    = v.value("masterBrightness", 1.0f);
        mPlexusThresh    = v.value("plexusThresh", 150.0f);
        typoEnabled      = v.value("typoEnabled", false);
    }

    lastPresetName = name;
    ofLogNotice("PRESET") << "loaded: " << path;
}

// ── List Presets ──────────────────────────────────────────────────────────────
std::vector<std::string> ofApp::listPresets() {
    std::vector<std::string> names;
    ofDirectory dir("presets");
    dir.allowExt("json");
    dir.listDir();
    for (auto& f : dir) {
        names.push_back(f.getBaseName());
    }
    return names;
}

// ── Switch Audio Device ───────────────────────────────────────────────────────
void ofApp::switchAudioDevice(int index) {
    if (index < 0 || index >= (int)inputDevices.size()) return;

    soundStream.close();

    ofSoundStreamSettings ss;
    ss.setInListener(this);
    ss.setInDevice(inputDevices[index]);
    ss.sampleRate = (int)SAMPLE_RATE;
    ss.numInputChannels = 1;
    ss.numOutputChannels = 0;
    ss.bufferSize = BUFFER_SIZE;
    soundStream.setup(ss);

    currentAudioDevice = index;
    ofLogNotice("AUDIO") << "switched to: " << inputDevices[index].name;
}
