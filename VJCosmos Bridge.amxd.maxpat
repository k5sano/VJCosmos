{
	"patcher": {
		"fileversion": 1,
		"appversion": {
			"major": 8,
			"minor": 6,
			"revision": 0,
			"architecture": "x64",
			"modernui": 1
		},
		"classnamespace": "box",
		"rect": [
			100,
			100,
			1400,
			900
		],
		"bglocked": 0,
		"openinpresentation": 1,
		"default_fontsize": 12.0,
		"default_fontname": "Arial",
		"gridsize": [
			15.0,
			15.0
		],
		"boxes": [
			{
				"box": {
					"id": "obj-1",
					"maxclass": "newobj",
					"text": "adc~ 1 2",
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"signal",
						"signal"
					],
					"patching_rect": [
						50,
						50,
						80,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-2",
					"maxclass": "newobj",
					"text": "dac~ 1 2",
					"numinlets": 2,
					"numoutlets": 1,
					"patching_rect": [
						50,
						120,
						80,
						22
					],
					"outlettype": [
						"signal"
					]
				}
			},
			{
				"box": {
					"id": "obj-3",
					"maxclass": "newobj",
					"text": "+~",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						"signal"
					],
					"patching_rect": [
						200,
						80,
						30,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-4",
					"maxclass": "newobj",
					"text": "*~ 0.5",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						"signal"
					],
					"patching_rect": [
						200,
						110,
						50,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-5",
					"maxclass": "newobj",
					"text": "lores~ 250 0.7",
					"numinlets": 3,
					"numoutlets": 1,
					"outlettype": [
						"signal"
					],
					"patching_rect": [
						200,
						150,
						90,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-6",
					"maxclass": "newobj",
					"text": "*~",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						"signal"
					],
					"patching_rect": [
						200,
						180,
						30,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-7",
					"maxclass": "newobj",
					"text": "peakamp~ 50",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						"float"
					],
					"patching_rect": [
						200,
						210,
						80,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-8",
					"maxclass": "newobj",
					"text": "svf~ 2000 0.5",
					"numinlets": 3,
					"numoutlets": 4,
					"outlettype": [
						"signal",
						"signal",
						"signal",
						"signal"
					],
					"patching_rect": [
						340,
						150,
						90,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-9",
					"maxclass": "newobj",
					"text": "*~",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						"signal"
					],
					"patching_rect": [
						340,
						180,
						30,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-10",
					"maxclass": "newobj",
					"text": "peakamp~ 50",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						"float"
					],
					"patching_rect": [
						340,
						210,
						80,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-11",
					"maxclass": "newobj",
					"text": "svf~ 4000 0.5",
					"numinlets": 3,
					"numoutlets": 4,
					"outlettype": [
						"signal",
						"signal",
						"signal",
						"signal"
					],
					"patching_rect": [
						480,
						150,
						90,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-12",
					"maxclass": "newobj",
					"text": "*~",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						"signal"
					],
					"patching_rect": [
						480,
						180,
						30,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-13",
					"maxclass": "newobj",
					"text": "peakamp~ 50",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						"float"
					],
					"patching_rect": [
						480,
						210,
						80,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-14",
					"maxclass": "newobj",
					"text": "peakamp~ 50",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						"float"
					],
					"patching_rect": [
						620,
						210,
						80,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-15",
					"maxclass": "newobj",
					"text": "metro 16",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						"bang"
					],
					"patching_rect": [
						50,
						260,
						60,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-16",
					"maxclass": "newobj",
					"text": "loadbang",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						"bang"
					],
					"patching_rect": [
						50,
						230,
						55,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-17",
					"maxclass": "message",
					"text": "1",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						50,
						245,
						20,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-18",
					"maxclass": "newobj",
					"text": "udpsend 127.0.0.1 9000",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						50,
						780,
						140,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-20",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/bass",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						200,
						290,
						135,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-21",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/mid",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						340,
						290,
						130,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-22",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/high",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						480,
						290,
						135,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-23",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/rms",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						620,
						290,
						130,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-24",
					"maxclass": "newobj",
					"text": "expr ($f3 / ($f1 + $f2 + $f3 + 0.001))",
					"numinlets": 3,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						760,
						250,
						220,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-25",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/centroid",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						760,
						290,
						155,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-26",
					"maxclass": "newobj",
					"text": "delay 16",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						"bang"
					],
					"patching_rect": [
						620,
						230,
						55,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-27",
					"maxclass": "flonum",
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"bang"
					],
					"patching_rect": [
						620,
						250,
						50,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-28",
					"maxclass": "newobj",
					"text": "t f f",
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"float",
						"float"
					],
					"patching_rect": [
						950,
						210,
						30,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-29",
					"maxclass": "newobj",
					"text": "- 0.",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						"float"
					],
					"patching_rect": [
						950,
						240,
						30,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-30",
					"maxclass": "newobj",
					"text": "abs",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						"int"
					],
					"patching_rect": [
						950,
						270,
						30,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-31",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/flux",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						950,
						300,
						135,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-32",
					"maxclass": "newobj",
					"text": "> 0.05",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						"int"
					],
					"patching_rect": [
						1100,
						270,
						45,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-33",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/onset",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						1100,
						300,
						140,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-34",
					"maxclass": "flonum",
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"bang"
					],
					"patching_rect": [
						200,
						250,
						50,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-35",
					"maxclass": "flonum",
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"bang"
					],
					"patching_rect": [
						340,
						250,
						50,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-36",
					"maxclass": "flonum",
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"bang"
					],
					"patching_rect": [
						480,
						250,
						50,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-37",
					"maxclass": "flonum",
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"bang"
					],
					"patching_rect": [
						950,
						180,
						50,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-100",
					"maxclass": "comment",
					"text": "VJCosmos Bridge",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						50,
						350,
						200,
						24
					],
					"presentation": 1,
					"presentation_rect": [
						10,
						5,
						200,
						24
					],
					"fontface": 1,
					"fontsize": 16
				}
			},
			{
				"box": {
					"id": "obj-101",
					"maxclass": "comment",
					"text": "FLUID",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						50,
						370,
						60,
						20
					],
					"presentation": 1,
					"presentation_rect": [
						10,
						35,
						60,
						20
					],
					"fontface": 1,
					"fontsize": 11
				}
			},
			{
				"box": {
					"id": "obj-102",
					"maxclass": "comment",
					"text": "BLOOM",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						200,
						370,
						60,
						20
					],
					"presentation": 1,
					"presentation_rect": [
						160,
						35,
						60,
						20
					],
					"fontface": 1,
					"fontsize": 11
				}
			},
			{
				"box": {
					"id": "obj-103",
					"maxclass": "comment",
					"text": "AUDIO",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						350,
						370,
						60,
						20
					],
					"presentation": 1,
					"presentation_rect": [
						270,
						35,
						60,
						20
					],
					"fontface": 1,
					"fontsize": 11
				}
			},
			{
				"box": {
					"id": "obj-104",
					"maxclass": "comment",
					"text": "VISUALS",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						50,
						470,
						70,
						20
					],
					"presentation": 1,
					"presentation_rect": [
						10,
						145,
						70,
						20
					],
					"fontface": 1,
					"fontsize": 11
				}
			},
			{
				"box": {
					"id": "obj-105",
					"maxclass": "comment",
					"text": "EFFECTS",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						50,
						570,
						70,
						20
					],
					"presentation": 1,
					"presentation_rect": [
						10,
						265,
						70,
						20
					],
					"fontface": 1,
					"fontsize": 11
				}
			},
			{
				"box": {
					"id": "obj-106",
					"maxclass": "comment",
					"text": "EFFECT PARAMS",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						50,
						620,
						100,
						20
					],
					"presentation": 1,
					"presentation_rect": [
						10,
						310,
						100,
						20
					],
					"fontface": 1,
					"fontsize": 11
				}
			},
			{
				"box": {
					"id": "obj-107",
					"maxclass": "comment",
					"text": "PALETTE",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						500,
						620,
						70,
						20
					],
					"presentation": 1,
					"presentation_rect": [
						370,
						310,
						70,
						20
					],
					"fontface": 1,
					"fontsize": 11
				}
			},
			{
				"box": {
					"id": "obj-200",
					"maxclass": "live.dial",
					"varname": "dissipation",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						50,
						390,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						10,
						55,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "dissipation",
							"parameter_shortname": "Dissip",
							"parameter_type": 0,
							"parameter_mmin": 0.9,
							"parameter_mmax": 1.0,
							"parameter_initial": [
								0.999
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-201",
					"maxclass": "live.dial",
					"varname": "velDissipation",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						100,
						390,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						60,
						55,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "velDissipation",
							"parameter_shortname": "VelDis",
							"parameter_type": 0,
							"parameter_mmin": 0.8,
							"parameter_mmax": 1.0,
							"parameter_initial": [
								0.99
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-202",
					"maxclass": "live.dial",
					"varname": "gravity",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						150,
						390,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						110,
						55,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "gravity",
							"parameter_shortname": "Gravity",
							"parameter_type": 0,
							"parameter_mmin": -0.05,
							"parameter_mmax": 0.05,
							"parameter_initial": [
								-0.0098
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-203",
					"maxclass": "live.dial",
					"varname": "bloomIntensity",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						200,
						390,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						160,
						55,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "bloomIntensity",
							"parameter_shortname": "BlmInt",
							"parameter_type": 0,
							"parameter_mmin": 0.0,
							"parameter_mmax": 2.0,
							"parameter_initial": [
								0.5
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-204",
					"maxclass": "live.dial",
					"varname": "bloomThreshold",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						250,
						390,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						210,
						55,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "bloomThreshold",
							"parameter_shortname": "BlmThr",
							"parameter_type": 0,
							"parameter_mmin": 0.1,
							"parameter_mmax": 0.8,
							"parameter_initial": [
								0.3
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-205",
					"maxclass": "live.dial",
					"varname": "bassSens",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						350,
						390,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						270,
						55,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "bassSens",
							"parameter_shortname": "BassSn",
							"parameter_type": 0,
							"parameter_mmin": 0.5,
							"parameter_mmax": 3.0,
							"parameter_initial": [
								1.0
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-206",
					"maxclass": "live.dial",
					"varname": "midSens",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						400,
						390,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						320,
						55,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "midSens",
							"parameter_shortname": "MidSn",
							"parameter_type": 0,
							"parameter_mmin": 0.5,
							"parameter_mmax": 3.0,
							"parameter_initial": [
								1.0
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-207",
					"maxclass": "live.dial",
					"varname": "highSens",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						450,
						390,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						370,
						55,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "highSens",
							"parameter_shortname": "HighSn",
							"parameter_type": 0,
							"parameter_mmin": 0.5,
							"parameter_mmax": 3.0,
							"parameter_initial": [
								1.0
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-210",
					"maxclass": "live.dial",
					"varname": "rotSpeed",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						50,
						490,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						10,
						165,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "rotSpeed",
							"parameter_shortname": "RotSpd",
							"parameter_type": 0,
							"parameter_mmin": 0.0,
							"parameter_mmax": 60.0,
							"parameter_initial": [
								15.0
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-211",
					"maxclass": "live.dial",
					"varname": "displacement",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						100,
						490,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						60,
						165,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "displacement",
							"parameter_shortname": "Displ",
							"parameter_type": 0,
							"parameter_mmin": 0.0,
							"parameter_mmax": 2.0,
							"parameter_initial": [
								1.0
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-212",
					"maxclass": "live.dial",
					"varname": "glitchFreq",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						150,
						490,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						110,
						165,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "glitchFreq",
							"parameter_shortname": "GltFrq",
							"parameter_type": 0,
							"parameter_mmin": 0.0,
							"parameter_mmax": 3.0,
							"parameter_initial": [
								1.0
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-213",
					"maxclass": "live.dial",
					"varname": "fluidSaturation",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						200,
						490,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						160,
						165,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "fluidSaturation",
							"parameter_shortname": "FldSat",
							"parameter_type": 0,
							"parameter_mmin": 0.0,
							"parameter_mmax": 1.0,
							"parameter_initial": [
								1.0
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-214",
					"maxclass": "live.dial",
					"varname": "plexusSpeed",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						250,
						490,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						210,
						165,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "plexusSpeed",
							"parameter_shortname": "PlxSpd",
							"parameter_type": 0,
							"parameter_mmin": 0.1,
							"parameter_mmax": 3.0,
							"parameter_initial": [
								1.0
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-215",
					"maxclass": "live.dial",
					"varname": "masterBrightness",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						300,
						490,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						260,
						165,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "masterBrightness",
							"parameter_shortname": "MstBrt",
							"parameter_type": 0,
							"parameter_mmin": 0.0,
							"parameter_mmax": 2.0,
							"parameter_initial": [
								1.0
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-300",
					"maxclass": "live.toggle",
					"varname": "fxKaleido",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						50,
						585,
						15,
						15
					],
					"presentation": 1,
					"presentation_rect": [
						15,
						285,
						15,
						15
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "fxKaleido",
							"parameter_shortname": "Kaleido",
							"parameter_type": 2,
							"parameter_mmin": 0.0,
							"parameter_mmax": 1.0,
							"parameter_initial": [
								0
							],
							"parameter_initial_enable": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-301",
					"maxclass": "live.toggle",
					"varname": "fxCrt",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						100,
						585,
						15,
						15
					],
					"presentation": 1,
					"presentation_rect": [
						65,
						285,
						15,
						15
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "fxCrt",
							"parameter_shortname": "CRT",
							"parameter_type": 2,
							"parameter_mmin": 0.0,
							"parameter_mmax": 1.0,
							"parameter_initial": [
								0
							],
							"parameter_initial_enable": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-302",
					"maxclass": "live.toggle",
					"varname": "fxWave",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						150,
						585,
						15,
						15
					],
					"presentation": 1,
					"presentation_rect": [
						115,
						285,
						15,
						15
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "fxWave",
							"parameter_shortname": "Wave",
							"parameter_type": 2,
							"parameter_mmin": 0.0,
							"parameter_mmax": 1.0,
							"parameter_initial": [
								0
							],
							"parameter_initial_enable": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-303",
					"maxclass": "live.toggle",
					"varname": "fxGlitch",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						200,
						585,
						15,
						15
					],
					"presentation": 1,
					"presentation_rect": [
						165,
						285,
						15,
						15
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "fxGlitch",
							"parameter_shortname": "Glitch",
							"parameter_type": 2,
							"parameter_mmin": 0.0,
							"parameter_mmax": 1.0,
							"parameter_initial": [
								0
							],
							"parameter_initial_enable": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-304",
					"maxclass": "live.toggle",
					"varname": "fxEdge",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						250,
						585,
						15,
						15
					],
					"presentation": 1,
					"presentation_rect": [
						215,
						285,
						15,
						15
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "fxEdge",
							"parameter_shortname": "Edge",
							"parameter_type": 2,
							"parameter_mmin": 0.0,
							"parameter_mmax": 1.0,
							"parameter_initial": [
								0
							],
							"parameter_initial_enable": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-305",
					"maxclass": "live.toggle",
					"varname": "fxMono",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						300,
						585,
						15,
						15
					],
					"presentation": 1,
					"presentation_rect": [
						265,
						285,
						15,
						15
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "fxMono",
							"parameter_shortname": "Mono",
							"parameter_type": 2,
							"parameter_mmin": 0.0,
							"parameter_mmax": 1.0,
							"parameter_initial": [
								0
							],
							"parameter_initial_enable": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-306",
					"maxclass": "live.toggle",
					"varname": "fxMirror",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						350,
						585,
						15,
						15
					],
					"presentation": 1,
					"presentation_rect": [
						315,
						285,
						15,
						15
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "fxMirror",
							"parameter_shortname": "Mirror",
							"parameter_type": 2,
							"parameter_mmin": 0.0,
							"parameter_mmax": 1.0,
							"parameter_initial": [
								0
							],
							"parameter_initial_enable": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-108",
					"maxclass": "comment",
					"text": "Kaleido",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						50,
						602,
						45,
						18
					],
					"presentation": 1,
					"presentation_rect": [
						5,
						300,
						45,
						18
					],
					"fontsize": 9
				}
			},
			{
				"box": {
					"id": "obj-109",
					"maxclass": "comment",
					"text": "CRT",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						100,
						602,
						30,
						18
					],
					"presentation": 1,
					"presentation_rect": [
						58,
						300,
						30,
						18
					],
					"fontsize": 9
				}
			},
			{
				"box": {
					"id": "obj-110",
					"maxclass": "comment",
					"text": "Wave",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						150,
						602,
						35,
						18
					],
					"presentation": 1,
					"presentation_rect": [
						107,
						300,
						35,
						18
					],
					"fontsize": 9
				}
			},
			{
				"box": {
					"id": "obj-111",
					"maxclass": "comment",
					"text": "Glitch",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						200,
						602,
						40,
						18
					],
					"presentation": 1,
					"presentation_rect": [
						155,
						300,
						40,
						18
					],
					"fontsize": 9
				}
			},
			{
				"box": {
					"id": "obj-112",
					"maxclass": "comment",
					"text": "Edge",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						250,
						602,
						35,
						18
					],
					"presentation": 1,
					"presentation_rect": [
						207,
						300,
						35,
						18
					],
					"fontsize": 9
				}
			},
			{
				"box": {
					"id": "obj-113",
					"maxclass": "comment",
					"text": "Mono",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						300,
						602,
						35,
						18
					],
					"presentation": 1,
					"presentation_rect": [
						257,
						300,
						35,
						18
					],
					"fontsize": 9
				}
			},
			{
				"box": {
					"id": "obj-114",
					"maxclass": "comment",
					"text": "Mirror",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						350,
						602,
						40,
						18
					],
					"presentation": 1,
					"presentation_rect": [
						305,
						300,
						40,
						18
					],
					"fontsize": 9
				}
			},
			{
				"box": {
					"id": "obj-220",
					"maxclass": "live.dial",
					"varname": "waveAmplitude",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						50,
						640,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						10,
						330,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "waveAmplitude",
							"parameter_shortname": "WavAmp",
							"parameter_type": 0,
							"parameter_mmin": 0.0,
							"parameter_mmax": 50.0,
							"parameter_initial": [
								10.0
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-221",
					"maxclass": "live.dial",
					"varname": "waveSpeed",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						100,
						640,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						60,
						330,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "waveSpeed",
							"parameter_shortname": "WavSpd",
							"parameter_type": 0,
							"parameter_mmin": 0.0,
							"parameter_mmax": 10.0,
							"parameter_initial": [
								2.0
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-222",
					"maxclass": "live.dial",
					"varname": "edgeStrength",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						150,
						640,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						110,
						330,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "edgeStrength",
							"parameter_shortname": "EdgeSt",
							"parameter_type": 0,
							"parameter_mmin": 0.0,
							"parameter_mmax": 3.0,
							"parameter_initial": [
								1.0
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-223",
					"maxclass": "live.dial",
					"varname": "edgeMix",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						200,
						640,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						160,
						330,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "edgeMix",
							"parameter_shortname": "EdgeMx",
							"parameter_type": 0,
							"parameter_mmin": 0.0,
							"parameter_mmax": 1.0,
							"parameter_initial": [
								0.3
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-224",
					"maxclass": "live.dial",
					"varname": "monoMode",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						250,
						640,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						210,
						330,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "monoMode",
							"parameter_shortname": "MonoMd",
							"parameter_type": 0,
							"parameter_mmin": 0.0,
							"parameter_mmax": 1.0,
							"parameter_initial": [
								0.0
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-225",
					"maxclass": "live.dial",
					"varname": "mirrorMode",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						300,
						640,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						260,
						330,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "mirrorMode",
							"parameter_shortname": "MirrMd",
							"parameter_type": 0,
							"parameter_mmin": 0.0,
							"parameter_mmax": 2.0,
							"parameter_initial": [
								0.0
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 1
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-226",
					"maxclass": "live.dial",
					"varname": "colorPalette",
					"parameter_enable": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"float"
					],
					"patching_rect": [
						500,
						640,
						44,
						48
					],
					"presentation": 1,
					"presentation_rect": [
						370,
						330,
						44,
						48
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_longname": "colorPalette",
							"parameter_shortname": "Palette",
							"parameter_type": 1,
							"parameter_mmin": 0,
							"parameter_mmax": 3,
							"parameter_initial": [
								3
							],
							"parameter_initial_enable": 1,
							"parameter_unitstyle": 0
						}
					}
				}
			},
			{
				"box": {
					"id": "obj-400",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/dissipation",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						50,
						450,
						210,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-401",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/velDissipation",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						270,
						450,
						225,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-402",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/gravity",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						505,
						450,
						190,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-403",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/bloomIntensity",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						705,
						450,
						225,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-404",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/bloomThreshold",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						940,
						450,
						230,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-405",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/bassSens",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						50,
						550,
						200,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-406",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/midSens",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						260,
						550,
						195,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-407",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/highSens",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						465,
						550,
						200,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-410",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/rotSpeed",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						675,
						550,
						200,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-411",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/displacement",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						885,
						550,
						220,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-412",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/glitchFreq",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						50,
						700,
						210,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-413",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/fluidSaturation",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						270,
						700,
						230,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-414",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/plexusSpeed",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						510,
						700,
						215,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-415",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/masterBrightness",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						735,
						700,
						240,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-500",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/fxKaleido",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						50,
						740,
						200,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-501",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/fxCrt",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						260,
						740,
						175,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-502",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/fxWave",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						445,
						740,
						185,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-503",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/fxGlitch",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						640,
						740,
						195,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-504",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/fxEdge",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						845,
						740,
						185,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-505",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/fxMono",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						1040,
						740,
						185,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-506",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/fxMirror",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						1235,
						740,
						195,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-520",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/waveAmplitude",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						50,
						760,
						230,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-521",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/waveSpeed",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						290,
						760,
						210,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-522",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/edgeStrength",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						510,
						760,
						220,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-523",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/edgeMix",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						740,
						760,
						195,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-524",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/monoMode",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						945,
						760,
						205,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-525",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/mirrorMode",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						1160,
						760,
						210,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-526",
					"maxclass": "newobj",
					"text": "prepend /vjcosmos/param/colorPalette",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						1380,
						760,
						215,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-115",
					"maxclass": "comment",
					"text": "Audio Analysis",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						200,
						30,
						100,
						20
					],
					"presentation": 1,
					"presentation_rect": [
						430,
						5,
						100,
						20
					],
					"fontface": 1,
					"fontsize": 11
				}
			},
			{
				"box": {
					"id": "obj-116",
					"maxclass": "comment",
					"text": "OSC: 127.0.0.1:9000",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						200,
						780,
						130,
						20
					],
					"presentation": 1,
					"presentation_rect": [
						430,
						20,
						130,
						20
					],
					"fontsize": 10
				}
			},
			{
				"box": {
					"id": "obj-117",
					"maxclass": "comment",
					"text": "~60fps OSC",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						50,
						800,
						80,
						20
					],
					"presentation": 1,
					"presentation_rect": [
						560,
						5,
						80,
						20
					],
					"fontsize": 10
				}
			},
			{
				"box": {
					"id": "obj-900",
					"maxclass": "newobj",
					"text": "print OSC-DEBUG",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						600,
						600,
						100,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-700",
					"maxclass": "newobj",
					"text": "scale 0. 1. 0.9 1.0",
					"numinlets": 6,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						50,
						445,
						120,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-701",
					"maxclass": "newobj",
					"text": "scale 0. 1. 0.8 1.0",
					"numinlets": 6,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						100,
						445,
						120,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-702",
					"maxclass": "newobj",
					"text": "scale 0. 1. -0.05 0.05",
					"numinlets": 6,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						150,
						445,
						120,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-703",
					"maxclass": "newobj",
					"text": "scale 0. 1. 0.0 2.0",
					"numinlets": 6,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						200,
						445,
						120,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-704",
					"maxclass": "newobj",
					"text": "scale 0. 1. 0.1 0.8",
					"numinlets": 6,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						250,
						445,
						120,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-705",
					"maxclass": "newobj",
					"text": "scale 0. 1. 0.5 3.0",
					"numinlets": 6,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						350,
						445,
						120,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-706",
					"maxclass": "newobj",
					"text": "scale 0. 1. 0.5 3.0",
					"numinlets": 6,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						400,
						445,
						120,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-707",
					"maxclass": "newobj",
					"text": "scale 0. 1. 0.5 3.0",
					"numinlets": 6,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						450,
						445,
						120,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-708",
					"maxclass": "newobj",
					"text": "scale 0. 1. 0.0 60.0",
					"numinlets": 6,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						50,
						545,
						120,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-709",
					"maxclass": "newobj",
					"text": "scale 0. 1. 0.0 2.0",
					"numinlets": 6,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						100,
						545,
						120,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-710",
					"maxclass": "newobj",
					"text": "scale 0. 1. 0.0 3.0",
					"numinlets": 6,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						150,
						545,
						120,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-711",
					"maxclass": "newobj",
					"text": "scale 0. 1. 0.1 3.0",
					"numinlets": 6,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						250,
						545,
						120,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-712",
					"maxclass": "newobj",
					"text": "scale 0. 1. 0.0 2.0",
					"numinlets": 6,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						300,
						545,
						120,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-713",
					"maxclass": "newobj",
					"text": "scale 0. 1. 0.0 50.0",
					"numinlets": 6,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						50,
						695,
						120,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-714",
					"maxclass": "newobj",
					"text": "scale 0. 1. 0.0 10.0",
					"numinlets": 6,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						100,
						695,
						120,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-715",
					"maxclass": "newobj",
					"text": "scale 0. 1. 0.0 3.0",
					"numinlets": 6,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						150,
						695,
						120,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-716",
					"maxclass": "newobj",
					"text": "scale 0. 1. 0.0 2.0",
					"numinlets": 6,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						300,
						695,
						120,
						22
					]
				}
			},
			{
				"box": {
					"id": "obj-717",
					"maxclass": "newobj",
					"text": "scale 0. 1. 0 3",
					"numinlets": 6,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						500,
						695,
						120,
						22
					]
				}
			}
		],
		"lines": [
			{
				"patchline": {
					"source": [
						"obj-1",
						0
					],
					"destination": [
						"obj-2",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-1",
						1
					],
					"destination": [
						"obj-2",
						1
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-1",
						0
					],
					"destination": [
						"obj-3",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-1",
						1
					],
					"destination": [
						"obj-3",
						1
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-3",
						0
					],
					"destination": [
						"obj-4",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-4",
						0
					],
					"destination": [
						"obj-5",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-5",
						0
					],
					"destination": [
						"obj-6",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-5",
						0
					],
					"destination": [
						"obj-6",
						1
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-6",
						0
					],
					"destination": [
						"obj-7",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-4",
						0
					],
					"destination": [
						"obj-8",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-8",
						1
					],
					"destination": [
						"obj-9",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-8",
						1
					],
					"destination": [
						"obj-9",
						1
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-9",
						0
					],
					"destination": [
						"obj-10",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-4",
						0
					],
					"destination": [
						"obj-11",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-11",
						2
					],
					"destination": [
						"obj-12",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-11",
						2
					],
					"destination": [
						"obj-12",
						1
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-12",
						0
					],
					"destination": [
						"obj-13",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-4",
						0
					],
					"destination": [
						"obj-14",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-16",
						0
					],
					"destination": [
						"obj-17",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-17",
						0
					],
					"destination": [
						"obj-15",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-7",
						0
					],
					"destination": [
						"obj-34",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-34",
						0
					],
					"destination": [
						"obj-20",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-10",
						0
					],
					"destination": [
						"obj-35",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-35",
						0
					],
					"destination": [
						"obj-21",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-13",
						0
					],
					"destination": [
						"obj-36",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-36",
						0
					],
					"destination": [
						"obj-22",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-14",
						0
					],
					"destination": [
						"obj-27",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-27",
						0
					],
					"destination": [
						"obj-23",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-34",
						0
					],
					"destination": [
						"obj-24",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-35",
						0
					],
					"destination": [
						"obj-24",
						1
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-36",
						0
					],
					"destination": [
						"obj-24",
						2
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-24",
						0
					],
					"destination": [
						"obj-25",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-27",
						0
					],
					"destination": [
						"obj-37",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-37",
						0
					],
					"destination": [
						"obj-28",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-28",
						0
					],
					"destination": [
						"obj-29",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-28",
						1
					],
					"destination": [
						"obj-29",
						1
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-29",
						0
					],
					"destination": [
						"obj-30",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-30",
						0
					],
					"destination": [
						"obj-31",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-30",
						0
					],
					"destination": [
						"obj-32",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-32",
						0
					],
					"destination": [
						"obj-33",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-20",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-21",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-22",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-23",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-25",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-31",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-33",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-200",
						1
					],
					"destination": [
						"obj-700",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-201",
						1
					],
					"destination": [
						"obj-701",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-202",
						1
					],
					"destination": [
						"obj-702",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-203",
						1
					],
					"destination": [
						"obj-703",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-204",
						1
					],
					"destination": [
						"obj-704",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-205",
						1
					],
					"destination": [
						"obj-705",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-206",
						1
					],
					"destination": [
						"obj-706",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-207",
						1
					],
					"destination": [
						"obj-707",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-210",
						1
					],
					"destination": [
						"obj-708",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-211",
						1
					],
					"destination": [
						"obj-709",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-212",
						1
					],
					"destination": [
						"obj-710",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-213",
						1
					],
					"destination": [
						"obj-413",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-214",
						1
					],
					"destination": [
						"obj-711",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-215",
						1
					],
					"destination": [
						"obj-712",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-300",
						0
					],
					"destination": [
						"obj-500",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-301",
						0
					],
					"destination": [
						"obj-501",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-302",
						0
					],
					"destination": [
						"obj-502",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-303",
						0
					],
					"destination": [
						"obj-503",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-304",
						0
					],
					"destination": [
						"obj-504",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-305",
						0
					],
					"destination": [
						"obj-505",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-306",
						0
					],
					"destination": [
						"obj-506",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-220",
						1
					],
					"destination": [
						"obj-713",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-221",
						1
					],
					"destination": [
						"obj-714",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-222",
						1
					],
					"destination": [
						"obj-715",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-223",
						1
					],
					"destination": [
						"obj-523",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-224",
						1
					],
					"destination": [
						"obj-524",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-225",
						1
					],
					"destination": [
						"obj-716",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-226",
						1
					],
					"destination": [
						"obj-717",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-400",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-401",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-402",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-403",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-404",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-405",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-406",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-407",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-410",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-411",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-412",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-413",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-414",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-415",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-500",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-501",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-502",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-503",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-504",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-505",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-506",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-520",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-521",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-522",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-523",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-524",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-525",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-526",
						0
					],
					"destination": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-400",
						0
					],
					"destination": [
						"obj-900",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-20",
						0
					],
					"destination": [
						"obj-900",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-700",
						0
					],
					"destination": [
						"obj-400",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-701",
						0
					],
					"destination": [
						"obj-401",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-702",
						0
					],
					"destination": [
						"obj-402",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-703",
						0
					],
					"destination": [
						"obj-403",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-704",
						0
					],
					"destination": [
						"obj-404",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-705",
						0
					],
					"destination": [
						"obj-405",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-706",
						0
					],
					"destination": [
						"obj-406",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-707",
						0
					],
					"destination": [
						"obj-407",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-708",
						0
					],
					"destination": [
						"obj-410",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-709",
						0
					],
					"destination": [
						"obj-411",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-710",
						0
					],
					"destination": [
						"obj-412",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-711",
						0
					],
					"destination": [
						"obj-414",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-712",
						0
					],
					"destination": [
						"obj-415",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-713",
						0
					],
					"destination": [
						"obj-520",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-714",
						0
					],
					"destination": [
						"obj-521",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-715",
						0
					],
					"destination": [
						"obj-522",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-716",
						0
					],
					"destination": [
						"obj-525",
						0
					]
				}
			},
			{
				"patchline": {
					"source": [
						"obj-717",
						0
					],
					"destination": [
						"obj-526",
						0
					]
				}
			}
		]
	}
}