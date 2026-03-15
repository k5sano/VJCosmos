#pragma once
#include "ofMain.h"
#include "ofxFluid.h"
#include "ofxFft.h"
#include "ofxMidi.h"
#include "ofxSyphon.h"
#include "ofxOsc.h"
#include <mutex>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <fstream>

// ── グリッチ多角形 ────────────────────────────────────────────────────────────
struct GlitchPolygon {
    std::vector<ofVec2f> vertices;
    ofColor  color, strokeColor;
    float    alpha, baseAlpha;
    float    lifetime, maxLifetime;
    float    rotation, rotSpeed;
    ofVec2f  position;
    float    scale;
    bool     filled;
    bool     chromatic;
    float    chromaticOffset;
};

// ── プレクサスパーティクル ────────────────────────────────────────────────────
struct PlexusParticle {
    ofVec3f pos, vel;
    float   radius, alpha;
};

// ── カラーパレット / メッシュジオメトリ ───────────────────────────────────────
enum ColorPalette { PALETTE_WARM = 0, PALETTE_COOL, PALETTE_MONO, PALETTE_RAINBOW, PALETTE_COUNT };
enum MeshGeometry { GEOM_ICOSPHERE = 0, GEOM_CUBE, GEOM_SPHERE, GEOM_COUNT };

// ── 座標ラベル ────────────────────────────────────────────────────────────────
struct CoordLabel {
    float   alpha       = 0;
    float   targetAlpha = 0;
    string  text;
    ofVec2f pos;
};

// ── エフェクトレイヤー ────────────────────────────────────────────────────────
struct ShaderLayer {
    string   name;
    ofShader shader;
    bool     enabled = false;
    int      key;
};

class ofApp : public ofBaseApp, public ofxMidiListener {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void keyPressed(int key) override;
    void audioIn(ofSoundBuffer& input) override;
    void windowResized(int w, int h) override;

    // ── Fluid ─────────────────────────────────────────────────────────────────
    ofxFluid      fluid;
    ofSoundStream soundStream;
    ofxFft*       fft = nullptr;
    bool          fluidRunning = true;

    // ── Bloom ─────────────────────────────────────────────────────────────────
    ofFbo    renderFbo, brightFbo, blurHFbo, blurVFbo;
    ofShader brightnessShader, blurShader, bloomShader;

    // ── Audio ─────────────────────────────────────────────────────────────────
    static const int   BUFFER_SIZE = 1024;
    std::mutex         audioMutex;
    std::vector<float> audioBuffer;
    float              rawRms = 0.0f;
    std::vector<float> prevAmplitude;

    float sBass = 0, sMid = 0, sHigh = 0, sRms = 0, sCentroid = 0, sFlux = 0;
    bool  onset = false, onsetRising = false;
    bool  prevOnset = false;

    // ── Glitch Polygons ───────────────────────────────────────────────────────
    std::vector<GlitchPolygon> polygons;
    float flashAlpha = 0.0f, bassSpawnCooldown = 0.0f, highSpawnCooldown = 0.0f;
    void spawnPolygons(int count, bool isOnset, float maxRadius = 150.0f);

    // ── 3D Wireframe ──────────────────────────────────────────────────────────
    ofMesh               sphereMesh;
    std::vector<glm::vec3> origVerts, origNorms;
    float                rotY = 0.0f, rotX = 0.0f, rotBoost = 1.0f;
    std::vector<ofVec2f> meshScreenPos;
    std::vector<float>   meshDisplacements;

    void setupWireframe();
    void updateWireframe(float dt, float t);
    void drawWireframe();

    // ── Plexus ────────────────────────────────────────────────────────────────
    std::vector<PlexusParticle> plexusParticles;
    std::vector<int>            plexusConnCounts;
    void setupPlexus();
    void updatePlexus(float dt, float t, float w, float h);
    void drawPlexusLayer(float w, float h);

    // ── OSC ──────────────────────────────────────────────────────────────────
    ofxOscReceiver oscReceiver;
    static const int OSC_PORT = 9000;
    void processOscMessages();
    bool  oscFftActive = false;
    float oscBass = 0, oscMid = 0, oscHigh = 0;
    float oscRms = 0, oscCentroid = 0, oscFlux = 0;
    bool  oscOnset = false;
    float oscFftTimeout = 0;
    std::map<std::string, float> oscParams;

    // ── OSC Debug ────────────────────────────────────────────────────────────
    bool  showOscDebug = false;
    int   oscMsgCount = 0;
    int   oscParamMsgCount = 0;
    int   oscFftMsgCount = 0;
    std::string lastOscAddr;
    float lastOscTime = 0;

    struct OscParamLog {
        float value = 0;
        float receivedAt = 0;
        bool  everReceived = false;
    };
    std::map<std::string, OscParamLog> oscParamLog;

    void  drawOscDebug();

    // ── MIDI ─────────────────────────────────────────────────────────────────
    ofxMidiIn             midiIn;
    void                  newMidiMessage(ofxMidiMessage& msg) override;
    std::mutex            midiMutex;
    std::map<int, float>  midiCC;
    std::map<int, bool>   midiNoteOn;
    ofxMidiMessage        lastMidiMsg;

    // ── MIDI マッピングパラメータ（補間済み） ────────────────────────────────────
    float mDissipation     = 0.999f;
    float mVelDissipation  = 0.99f;
    float mBloomIntensity  = 0.5f;
    float mBloomThreshold  = 0.3f;
    float mBassSens        = 1.0f;
    float mMidSens         = 1.0f;
    float mHighSens        = 1.0f;
    float mPlexusThresh    = 150.0f;
    float mRotSpeed        = 15.0f;
    float mDisplacement    = 1.0f;
    float mGlitchFreq      = 1.0f;
    float mGravity         = -0.0098f;
    float mFluidSaturation = 1.0f;
    float mPlexusSpeed     = 1.0f;
    float mReserved        = 0.0f;
    float mMasterBright    = 1.0f;

    bool midiTrigFluidClear  = false;
    bool midiTrigPaletteNext = false;
    bool midiTrigGlitchBurst = false;
    bool midiTrigMeshNext    = false;
    bool midiTrigScreenshot  = false;

    ColorPalette colorPalette = PALETTE_RAINBOW;
    MeshGeometry meshGeometry = GEOM_ICOSPHERE;
    bool plexusVisible = true;
    bool bloomEnabled  = true;
    bool frozen        = false;

    bool gravityUp   = false;
    bool gravityDown  = false;

    void applyMidiParams(float dt);
    ofFloatColor paletteColor(float t);
    void rebuildMesh();

    // ── Display / Fullscreen ─────────────────────────────────────────────────
    bool showDebug     = true;
    bool showCoordText = true;
    bool showHelp      = false;
    bool isFullscreen  = false;

    void reallocateFbos(int w, int h);

    // ── 座標ラベル ───────────────────────────────────────────────────────────
    std::map<int, CoordLabel> meshLabels;
    std::map<int, CoordLabel> plexusLabels;
    int   coordFrameCounter = 0;
    float coordGlitchTimer  = 0;

    void updateCoordLabels(float dt);
    void drawCoordLabels();

    // ── シーン合成・エフェクトチェーン FBO ────────────────────────────────────
    ofFbo sceneFbo;
    ofFbo effectFboA, effectFboB;

    // ── エフェクトレイヤー（適用順） ──────────────────────────────────────────
    static const int FX_KALEIDO = 0;
    static const int FX_WAVE    = 1;
    static const int FX_GLITCH  = 2;
    static const int FX_EDGE    = 3;
    static const int FX_MONO    = 4;
    static const int FX_MIRROR  = 5;
    static const int FX_CRT     = 6;
    static const int FX_COUNT   = 7;
    ShaderLayer fx[FX_COUNT];

    void setupEffects();
    void applyEffectChain(float w, float h);
    void updateEffectParams(float dt);

    // ── Kaleidoscope パラメータ ───────────────────────────────────────────────
    int      kaleidoSegments = 6;
    float    kaleidoRotation = 0;
    float    kaleidoRotBoost = 0;

    // ── CRT パラメータ ───────────────────────────────────────────────────────
    float    crtChromatic     = 1.5f;
    float    crtChromaticCur  = 1.5f;

    // ── Wave パラメータ ──────────────────────────────────────────────────────
    float    waveAmplitude  = 10.0f;
    float    waveFrequency  = 0.05f;
    float    waveSpeed      = 2.0f;
    float    waveSpeedCur   = 2.0f;

    // ── Glitch パラメータ ────────────────────────────────────────────────────
    float    glitchIntensity = 0.3f;
    float    glitchIntensityCur = 0.0f;
    float    glitchBlockSize = 30.0f;

    // ── Edge パラメータ ──────────────────────────────────────────────────────
    float    edgeStrength = 1.0f;
    float    edgeMix      = 0.3f;

    // ── Mono パラメータ ──────────────────────────────────────────────────────
    float    monoMode      = 0.0f;
    float    monoIntensity = 1.0f;
    float    monoIntensityCur = 1.0f;

    // ── Mirror パラメータ ────────────────────────────────────────────────────
    float    mirrorMode = 0.0f;

    // ── タイポグラフィ（テキスト→流体溶解）──────────────────────────────────
    bool              typoEnabled   = false;
    bool              typoFontOk    = false;
    ofTrueTypeFont    typoFont;
    ofFbo             typoFbo;
    std::vector<std::string> typoMessages;
    int               typoMsgIndex  = 0;
    float             typoTimer     = 0;
    bool              typoInject    = false;
    ofVec2f           typoPos;

    void setupTypography();
    void updateTypography(float dt, float w, float h);
    void injectTypography(float w, float h);

    // ── Syphon 出力 ──────────────────────────────────────────────────────────
    ofxSyphonServer syphonServer;

    // ── プリセット保存/ロード ────────────────────────────────────────────────
    string lastPresetName = "";
    void savePreset(const string& name);
    void loadPreset(const string& name);
    std::vector<std::string> listPresets();

    // ── オーディオデバイス選択 ────────────────────────────────────────────────
    std::vector<ofSoundDevice> inputDevices;
    int currentAudioDevice = 0;
    void switchAudioDevice(int index);

private:
    void drawPolygon(const GlitchPolygon& p, float offsetX,
                     const ofColor& col, float alphaScale);
};
