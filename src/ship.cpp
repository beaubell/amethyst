/*
This file was produced by Deep Exploration Plugin: CPP Export filter.

Deep Exploration

Copyright (c) 1999-2003  Right Hemisphere Ltd

http://www.righthemisphere.com/
support@righthemisphere.com
*/

#include "SDL_opengl.h"

// 107 Verticies
// 234 Texture Coordinates
// 76 Normals
// 156 Triangles

// Call this forward declared function to draw the ship
void DrawShip(void);

static GLint face_indicies[156][9] = {
// Object #-1
	{51,47,65 ,0,0,0 ,0,1,2 }, {65,69,51 ,0,0,0 ,2,3,0 }, {67,48,51 ,1,1,1 ,4,5,6 },
	{51,69,67 ,1,1,1 ,6,7,4 }, {86,85,27 ,2,2,2 ,8,9,10 }, {27,28,86 ,2,2,2 ,10,11,8 },
	{28,2,4 ,3,4,3 ,12,13,14 }, {4,29,28 ,3,5,3 ,14,15,12 }, {1,27,26 ,6,7,6 ,16,17,18 },
	{26,3,1 ,6,8,6 ,18,19,16 }, {85,105,102 ,9,10,9 ,20,21,22 },
	{102,84,85 ,9,11,9 ,22,23,20 }, {59,55,54 ,1,1,1 ,24,25,26 },
	{54,58,59 ,1,1,1 ,26,27,24 }, {62,44,45 ,12,12,12 ,28,29,30 },
	{45,61,62 ,12,12,12 ,30,31,28 }, {49,68,50 ,1,1,1 ,32,33,34 },
	{66,50,68 ,1,1,1 ,35,34,33 }, {86,28,29 ,1,1,1 ,36,37,38 }, {29,87,86 ,1,1,1 ,38,39,36 },
	{27,85,84 ,13,13,13 ,40,41,42 }, {84,26,27 ,13,13,13 ,42,43,40 },
	{52,57,56 ,14,14,14 ,44,45,46 }, {56,53,52 ,14,14,14 ,46,47,44 },
	{90,91,81 ,15,15,15 ,48,49,50 }, {81,80,90 ,15,15,15 ,50,51,48 },
	{21,22,14 ,15,15,15 ,52,53,54 }, {14,13,21 ,15,15,15 ,54,55,52 },
	{106,86,87 ,16,17,16 ,56,57,58 }, {87,103,106 ,16,18,16 ,58,59,56 },
	{93,92,82 ,19,19,19 ,60,61,62 }, {82,83,93 ,19,19,19 ,62,63,60 },
	{25,23,15 ,19,19,19 ,64,65,66 }, {15,16,25 ,19,19,19 ,66,67,64 },
	{7,12,5 ,20,20,20 ,68,69,70 }, {7,11,12 ,20,20,20 ,68,71,69 },
	{7,5,11 ,20,20,20 ,68,70,71 }, {96,100,95 ,20,20,20 ,72,73,74 },
	{96,98,100 ,20,20,20 ,72,75,73 }, {96,95,98 ,20,20,20 ,72,74,75 },
	{48,19,47 ,21,22,21 ,76,77,78 }, {47,51,48 ,21,23,21 ,78,79,76 },
	{88,67,69 ,24,25,24 ,77,80,81 }, {69,65,88 ,24,26,24 ,81,78,77 },
	{0,1,3 ,27,27,27 ,82,83,84 }, {3,4,0 ,27,27,27 ,84,85,82 }, {105,106,103 ,28,28,28 ,86,87,88 },
	{103,104,105 ,28,28,28 ,88,89,86 }, {55,52,53 ,23,23,23 ,90,91,92 },
	{53,54,55 ,23,23,23 ,92,93,90 }, {57,59,58 ,29,29,29 ,91,94,93 },
	{58,56,57 ,29,29,29 ,93,92,91 }, {93,91,90 ,29,29,29 ,95,96,97 },
	{90,92,93 ,29,29,29 ,97,98,95 }, {83,82,80 ,23,23,23 ,95,98,97 },
	{80,81,83 ,23,23,23 ,97,96,95 }, {44,43,42 ,23,30,31 ,99,100,101 },
	{42,45,44 ,31,23,23 ,101,102,99 }, {43,32,33 ,30,32,33 ,100,103,104 },
	{33,42,43 ,33,31,30 ,104,101,100 }, {78,63,60 ,34,35,36 ,103,100,105 },
	{60,79,78 ,36,37,34 ,105,106,103 }, {63,62,61 ,35,29,29 ,100,107,108 },
	{61,60,63 ,29,36,35 ,108,105,100 }, {25,22,21 ,29,29,29 ,95,96,97 },
	{21,23,25 ,29,29,29 ,97,109,95 }, {16,15,13 ,23,23,23 ,95,98,97 },
	{13,14,16 ,23,23,23 ,97,96,95 }, {20,49,46 ,38,39,39 ,110,111,112 },
	{50,46,49 ,23,39,39 ,113,112,111 }, {68,89,66 ,25,40,40 ,114,115,116 },
	{64,66,89 ,41,40,40 ,117,116,115 }, {106,105,85 ,42,43,42 ,118,119,120 },
	{85,86,106 ,42,44,42 ,120,121,118 }, {28,27,1 ,45,46,45 ,122,123,124 },
	{1,2,28 ,45,47,45 ,124,125,122 }, {103,87,84 ,48,49,50 ,126,127,128 },
	{84,102,103 ,50,48,48 ,128,129,126 }, {87,29,26 ,49,51,52 ,127,130,131 },
	{26,84,87 ,52,50,49 ,131,128,127 }, {29,4,3 ,51,53,53 ,130,132,133 },
	{3,26,29 ,53,52,51 ,133,131,130 }, {55,59,57 ,54,54,54 ,134,135,136 },
	{57,52,55 ,54,54,54 ,136,137,134 }, {54,53,56 ,55,55,55 ,138,139,140 },
	{56,58,54 ,55,55,55 ,140,141,138 }, {92,90,80 ,56,56,56 ,142,143,144 },
	{80,82,92 ,56,56,56 ,144,145,142 }, {91,93,83 ,57,57,57 ,146,147,148 },
	{83,81,91 ,57,57,57 ,148,149,146 }, {62,63,43 ,58,59,60 ,150,151,152 },
	{43,44,62 ,60,58,58 ,152,153,150 }, {63,78,32 ,59,61,61 ,151,154,155 },
	{32,43,63 ,61,60,59 ,155,152,151 }, {23,21,13 ,56,56,56 ,156,157,158 },
	{13,15,23 ,56,56,56 ,158,159,156 }, {24,25,16 ,57,57,57 ,160,161,162 },
	{16,14,24 ,57,57,57 ,162,163,160 }, {5,10,6 ,62,62,62 ,164,165,166 },
	{5,12,10 ,63,63,63 ,167,168,165 }, {12,9,10 ,29,29,29 ,169,165,165 },
	{12,11,9 ,29,29,29 ,169,170,165 }, {11,6,9 ,64,64,64 ,169,171,165 },
	{11,5,6 ,64,64,64 ,169,167,171 }, {8,6,10 ,1,1,1 ,172,171,165 },
	{8,10,9 ,1,1,1 ,172,165,165 }, {8,9,6 ,1,1,1 ,172,165,171 },
	{95,101,94 ,65,65,65 ,173,174,175 }, {95,100,101 ,66,66,66 ,176,177,178 },
	{100,99,101 ,29,29,29 ,179,178,178 }, {100,98,99 ,29,29,29 ,179,180,178 },
	{98,94,99 ,64,64,64 ,179,181,178 }, {98,95,94 ,64,64,64 ,179,176,181 },
	{97,94,101 ,1,1,1 ,182,181,178 }, {97,101,99 ,1,1,1 ,182,178,178 },
	{97,99,94 ,1,1,1 ,182,178,181 }, {35,39,34 ,66,66,66 ,183,184,185 },
	{35,41,39 ,67,67,67 ,186,187,188 }, {41,38,39 ,29,29,29 ,189,188,188 },
	{41,40,38 ,29,29,29 ,189,189,188 }, {40,34,38 ,68,68,68 ,189,190,188 },
	{40,35,34 ,68,68,68 ,189,186,190 }, {36,34,39 ,1,1,1 ,191,190,188 },
	{36,39,38 ,1,1,1 ,191,188,188 }, {36,38,34 ,1,1,1 ,191,188,190 },
	{71,75,70 ,66,66,66 ,192,193,194 }, {71,77,75 ,67,67,67 ,195,196,197 },
	{77,74,75 ,29,29,29 ,198,197,197 }, {77,76,74 ,29,29,29 ,198,198,197 },
	{76,70,74 ,68,68,68 ,198,194,197 }, {76,71,70 ,68,68,68 ,198,195,194 },
	{72,70,75 ,1,1,1 ,199,194,197 }, {72,75,74 ,1,1,1 ,199,197,197 },
	{72,74,70 ,1,1,1 ,199,197,194 }, {48,67,88 ,54,54,54 ,200,200,201 },
	{88,17,48 ,54,54,54 ,201,201,200 }, {60,42,31 ,69,69,69 ,202,203,204 },
	{31,79,60 ,69,69,69 ,204,205,202 }, {30,78,79 ,70,70,70 ,206,207,208 },
	{79,31,30 ,70,70,70 ,208,209,206 }, {37,41,35 ,20,20,20 ,210,211,212 },
	{37,40,41 ,20,20,20 ,210,213,211 }, {37,35,40 ,20,20,20 ,210,214,213 },
	{73,77,71 ,20,20,20 ,210,215,212 }, {73,76,77 ,20,20,20 ,210,213,215 },
	{73,71,76 ,20,20,20 ,210,214,213 }, {68,49,89 ,71,71,71 ,216,217,218 },
	{18,89,49 ,71,71,71 ,219,218,217 }, {19,88,65 ,72,72,72 ,220,221,222 },
	{65,47,19 ,72,72,72 ,222,223,220 }, {46,50,64 ,73,73,73 ,223,224,222 },
	{66,64,50 ,73,73,73 ,225,222,224 }, {61,45,42 ,74,74,74 ,226,227,228 },
	{42,60,61 ,74,74,74 ,228,229,226 }, {89,20,64 ,75,75,75 ,230,231,232 },
	{46,64,20 ,75,75,75 ,233,232,231 }
};
static GLfloat vertices [107][3] = {
{-2.87744f,-2.03548f,-1.00507f},{-2.87744f,-2.84922f,-1.00507f},{-2.87744f,-2.03548f,-1.00507f},
{-2.87066f,-2.84922f,-0.714153f},{-2.87066f,-2.03548f,-0.714153f},{-2.83428f,-2.58509f,-0.785277f},
{-2.83428f,0.39035f,-0.785276f},{-2.77265f,-2.58509f,-0.785277f},{-2.77265f,0.39035f,-0.785276f},
{-2.74183f,0.39035f,-0.838649f},{-2.74183f,0.39035f,-0.731902f},{-2.74183f,-2.58509f,-0.838651f},
{-2.74183f,-2.58509f,-0.731903f},{-1.17276f,-3.0174f,-0.550194f},{-1.17276f,-3.00927f,0.716256f},
{-1.17276f,-0.733242f,-0.193126f},{-1.17276f,-0.739993f,0.627137f},{-0.971649f,-1.58202f,0.091718f},
{-0.971649f,-1.58202f,0.093816f},{-0.971648f,-1.58202f,0.091718f},{-0.971648f,-1.58202f,0.093816f},
{-0.919622f,-3.0174f,-0.550194f},{-0.919622f,-3.00927f,0.716256f},{-0.919621f,-0.733242f,-0.193126f},
{-0.919621f,-3.00927f,0.716256f},{-0.919621f,-0.739993f,0.627137f},{-0.896981f,-2.24142f,0.608647f},
{-0.896981f,-1.89962f,-0.26927f},{-0.896981f,-0.971983f,0.301365f},{-0.896981f,-0.971983f,0.608647f},
{-0.828346f,-1.55051f,-0.193933f},{-0.828346f,-1.44057f,0.020239f},{-0.828345f,-1.55051f,-0.193933f},
{-0.828345f,-1.44057f,0.020239f},{-0.759736f,3.0174f,0.089051f},{-0.759736f,0.762811f,0.08905f},
{-0.698106f,3.0174f,0.089051f},{-0.698105f,0.762811f,0.08905f},{-0.66729f,3.0174f,0.035678f},
{-0.66729f,3.0174f,0.142425f},{-0.66729f,0.762811f,0.035677f},{-0.66729f,0.762811f,0.142424f},
{-0.595673f,-0.451212f,-0.553575f},{-0.595673f,-0.536022f,-0.798754f},{-0.595673f,-0.089443f,-0.802003f},
{-0.595673f,-0.096115f,-0.552213f},{-0.581825f,-0.443211f,-0.543283f},{-0.581825f,-0.443211f,0.728817f},
{-0.581825f,2.56343f,0.091717f},{-0.581825f,2.56343f,0.093815f},{-0.581825f,2.56343f,-0.026101f},
{-0.581825f,2.56343f,0.211633f},{-0.473029f,-2.1406f,0.597286f},{-0.473029f,-2.2498f,1.00507f},
{-0.473029f,-1.34519f,0.850424f},{-0.473029f,-1.34519f,0.597286f},{0.473028f,-2.2498f,1.00507f},
{0.473028f,-2.1406f,0.597286f},{0.473028f,-1.34519f,0.850424f},{0.473028f,-1.34519f,0.597286f},
{0.577384f,-0.451212f,-0.553575f},{0.577384f,-0.096115f,-0.552212f},{0.577384f,-0.089443f,-0.802003f},
{0.577384f,-0.536022f,-0.798754f},{0.581825f,-0.443212f,-0.543283f},{0.581825f,-0.443211f,0.728816f},
{0.581826f,2.56343f,-0.026101f},{0.581826f,2.56343f,0.091717f},{0.581826f,2.56343f,0.093815f},
{0.581826f,2.56343f,0.211632f},{0.67798f,3.0174f,0.089051f},{0.67798f,0.762811f,0.08905f},
{0.73961f,3.0174f,0.089051f},{0.739611f,0.762811f,0.08905f},{0.770426f,3.0174f,0.035678f},
{0.770426f,3.0174f,0.142425f},{0.770426f,0.762811f,0.035677f},{0.770426f,0.762811f,0.142424f},
{0.810057f,-1.55051f,-0.193933f},{0.810057f,-1.44057f,0.020239f},{0.888894f,-3.0174f,-0.550194f},
{0.888894f,-3.00927f,0.716256f},{0.888894f,-0.733242f,-0.193126f},{0.888894f,-0.739993f,0.627137f},
{0.89698f,-2.24142f,0.608647f},{0.89698f,-1.89963f,-0.26927f},{0.89698f,-0.971984f,0.301365f},
{0.89698f,-0.971984f,0.608647f},{0.971647f,-1.58202f,0.091718f},{0.971647f,-1.58202f,0.093816f},
{1.14203f,-3.0174f,-0.550194f},{1.14203f,-3.00927f,0.716256f},{1.14203f,-0.733242f,-0.193126f},
{1.14203f,-0.739993f,0.627137f},{2.71912f,0.39035f,-0.785276f},{2.71912f,-2.58509f,-0.785277f},
{2.78075f,-2.58509f,-0.785277f},{2.78075f,0.39035f,-0.785276f},{2.81157f,-2.58509f,-0.838651f},
{2.81157f,0.39035f,-0.838649f},{2.81157f,-2.58509f,-0.731903f},{2.81157f,0.39035f,-0.731902f},
{2.8398f,-2.84922f,-0.714153f},{2.8398f,-2.03548f,-0.714153f},{2.8398f,-2.84922f,-0.714153f},
{2.87744f,-2.84922f,-1.00507f},{2.87744f,-2.03548f,-1.00507f}
};
static GLfloat normals [76][3] = {
{0.0f,0.169524f,0.985526f},{0.0f,1.0f,0.0f},{0.0f,0.52395f,-0.851749f},
{-0.476576f,0.879116f,0.005585f},{-0.478776f,0.877866f,0.011171f},{-0.474357f,0.880332f,0.0f},
{0.40881f,-0.897845f,-0.16355f},{0.502754f,-0.805534f,-0.313614f},{0.298567f,-0.954363f,-0.006966f},
{-0.465953f,-0.864051f,-0.190535f},{-0.449091f,-0.891594f,-0.058108f},{-0.474158f,-0.820453f,-0.319423f},
{0.0f,0.999643f,0.026703f},{0.0f,-0.931867f,-0.362799f},{0.0f,-0.965964f,-0.258675f},
{0.0f,-0.999979f,0.006417f},{0.49373f,0.868979f,0.033251f},{0.473096f,0.881011f,0.0f},
{0.513535f,0.855492f,0.066447f},{0.0f,0.999966f,0.00823f},{0.0f,-1.0f,0.0f},
{-0.977414f,0.043808f,0.206741f},{-0.910678f,0.085637f,0.404143f},{-1.0f,0.0f,0.0f},
{0.972556f,0.085192f,0.21651f},{0.995608f,0.093623f,0.0f},{0.903329f,0.072717f,0.42274f},
{-0.999728f,0.0f,0.023326f},{0.991733f,0.0f,0.128321f},{1.0f,0.0f,-2e-006f},
{-0.991188f,0.121908f,-0.051824f},{-0.997688f,0.064231f,-0.022218f},{-0.981069f,0.172287f,-0.088432f},
{-0.980236f,0.182064f,-0.077397f},{0.980213f,0.182009f,-0.077821f},{0.997817f,0.062408f,-0.021587f},
{0.991177f,0.121872f,-0.052108f},{0.979877f,0.177578f,-0.091148f},{-0.910678f,0.085637f,-0.404143f},
{-0.977414f,0.043808f,-0.206741f},{0.972556f,0.085192f,-0.21651f},{0.903329f,0.072717f,-0.42274f},
{-0.320358f,0.261362f,-0.910528f},{-0.348268f,0.0f,-0.937395f},{-0.270083f,0.504478f,-0.820096f},
{0.329563f,0.279843f,-0.901707f},{0.065086f,0.522839f,-0.849943f},{0.550645f,0.0f,-0.83474f},
{0.5628f,0.0f,0.826593f},{0.195276f,0.0f,0.980748f},{0.390551f,0.0f,0.920581f},
{-0.385969f,0.0f,0.922512f},{-0.192984f,0.0f,0.981202f},{-0.556743f,0.0f,0.830685f},
{0.0f,0.0f,-1.0f},{0.0f,0.168506f,0.985701f},{0.0f,0.154448f,-0.988001f},
{0.0f,0.039242f,0.99923f},{0.0f,-0.007276f,-0.999974f},{0.0f,-0.354817f,-0.934936f},
{0.0f,-0.181163f,-0.983453f},{0.0f,-0.512085f,-0.858935f},{-0.500002f,0.0f,0.866024f},
{-0.499999f,-1e-006f,0.866026f},{-0.500001f,0.0f,-0.866025f},{-0.500001f,0.0f,0.866025f},
{-0.5f,0.0f,0.866025f},{-0.500001f,-1e-006f,0.866025f},{-0.5f,0.0f,-0.866025f},
{0.0f,0.501708f,0.865037f},{0.0f,-0.88965f,0.456643f},{0.0f,0.0f,1.0f},
{0.0f,-0.488232f,0.872714f},{0.0f,0.169524f,-0.985526f},{0.0f,-0.003837f,0.999993f},
{0.0f,-0.488233f,-0.872713f}
};
static GLfloat textures [234][2] = {
{0.846339f,0.673143f},{0.773436f,0.673143f},{0.773436f,0.509194f},
{0.846339f,0.509194f},{0.863242f,0.509194f},{0.863242f,0.673143f},
{0.846339f,0.673143f},{0.846339f,0.509194f},{0.83369f,0.464792f},
{0.914127f,0.464792f},{0.914127f,0.717545f},{0.83369f,0.717545f},
{0.83369f,0.717545f},{1.01785f,0.996576f},{0.976838f,0.995619f},
{0.790375f,0.717545f},{1.01785f,0.996576f},{0.914127f,0.717545f},
{0.790375f,0.717545f},{0.976838f,0.995619f},{0.914127f,0.464792f},
{1.01785f,0.185761f},{0.976838f,0.191065f},{0.790375f,0.464792f},
{0.791976f,0.524523f},{0.791976f,0.657814f},{0.756294f,0.657814f},
{0.756294f,0.524523f},{0.989221f,0.50982f},{0.989221f,0.675094f},
{0.954011f,0.675094f},{0.954011f,0.50982f},{0.862946f,0.673143f},
{0.862946f,0.509194f},{0.87985f,0.673143f},{0.87985f,0.509194f},
{0.57694f,0.623077f},{0.57694f,0.424975f},{0.546653f,0.424975f},
{0.546653f,0.623077f},{0.633183f,0.424975f},{0.633183f,0.623077f},
{0.546653f,0.623077f},{0.546653f,0.424975f},{0.547773f,0.471791f},
{0.547773f,0.576262f},{0.507581f,0.576262f},{0.507581f,0.471791f},
{0.660871f,0.650138f},{0.536047f,0.650138f},{0.536047f,0.622185f},
{0.660871f,0.622185f},{0.660871f,0.422475f},{0.536047f,0.422475f},
{0.536047f,0.394522f},{0.660871f,0.394522f},{1.01785f,0.185761f},
{0.83369f,0.464792f},{0.790375f,0.464792f},{0.976838f,0.191065f},
{0.787769f,0.430266f},{0.903394f,0.430266f},{0.903394f,0.465931f},
{0.787769f,0.465931f},{0.787769f,0.720735f},{0.903394f,0.720735f},
{0.903394f,0.7564f},{0.787769f,0.7564f},{0.986863f,0.981811f},
{0.97934f,0.97747f},{0.986863f,0.990494f},{0.994387f,0.97747f},
{0.986864f,0.199384f},{0.97934f,0.195042f},{0.986864f,0.208067f},
{0.994387f,0.195042f},{0.015138f,0.115077f},{0.594539f,0.115077f},
{0.43537f,0.028575f},{0.015138f,0.098795f},{0.015138f,0.115077f},
{0.015138f,0.098795f},{0.657918f,0.263992f},{0.771653f,0.263992f},
{0.771653f,0.224494f},{0.657918f,0.224494f},{0.771653f,0.263992f},
{0.657918f,0.263992f},{0.657918f,0.224493f},{0.771653f,0.224493f},
{0.561438f,0.046434f},{0.67261f,0.046434f},{0.687873f,-0.00893295f},
{0.561438f,0.012064f},{0.561438f,0.046434f},{0.476851f,0.042381f},
{0.794023f,0.030281f},{0.795159f,0.202232f},{0.475907f,0.153751f},
{0.385924f,0.236421f},{0.448342f,0.23598f},{0.436488f,0.202691f},
{0.386857f,0.202506f},{0.590134f,0.153861f},{0.574769f,0.124782f},
{0.436488f,0.202691f},{0.574769f,0.124782f},{0.385924f,0.236421f},
{0.386857f,0.202506f},{0.475907f,0.153751f},{0.594539f,0.114792f},
{0.015138f,0.114792f},{0.43537f,0.201294f},{0.015138f,0.131074f},
{0.015138f,0.114792f},{0.594539f,0.114792f},{0.015138f,0.131074f},
{0.43537f,0.201294f},{0.734673f,0.874576f},{0.734673f,0.97538f},
{0.489894f,0.857747f},{0.489894f,0.742834f},{0.268166f,0.742834f},
{0.268166f,0.857747f},{0.023386f,0.975379f},{0.023386f,0.874576f},
{0.730021f,0.874576f},{0.489894f,0.742834f},{0.489894f,0.900088f},
{0.730021f,0.97538f},{0.268166f,0.742834f},{0.268166f,0.900087f},
{0.024225f,0.874576f},{0.024225f,0.975379f},{0.320565f,0.789066f},
{0.437495f,0.789066f},{0.437495f,0.887598f},{0.320565f,0.887598f},
{0.320565f,0.789066f},{0.320565f,0.901125f},{0.437495f,0.901125f},
{0.437495f,0.789066f},{0.520182f,0.71326f},{0.520182f,0.996213f},
{0.488895f,0.996213f},{0.488895f,0.71326f},{0.520182f,0.995206f},
{0.520182f,0.714096f},{0.488895f,0.714096f},{0.488895f,0.995206f},
{0.450393f,0.633508f},{0.450393f,0.688829f},{0.305407f,0.688829f},
{0.305407f,0.633508f},{0.479151f,0.814499f},{0.276649f,0.814499f},
{0.265367f,0.71326f},{0.265367f,0.996213f},{0.23408f,0.996213f},
{0.23408f,0.71326f},{0.265367f,0.995206f},{0.265367f,0.714096f},
{0.23408f,0.714096f},{0.23408f,0.995206f},{0.028721f,0.94266f},
{0.040147f,0.574073f},{0.028721f,0.574073f},{0.028721f,0.94266f},
{0.040147f,0.94266f},{0.040147f,0.94266f},{0.040147f,0.94266f},
{0.028721f,0.574073f},{0.036339f,0.574073f},{0.715105f,0.94266f},
{0.726532f,0.574073f},{0.715106f,0.574073f},{0.715105f,0.94266f},
{0.726532f,0.94266f},{0.726532f,0.574073f},{0.726532f,0.94266f},
{0.726532f,0.94266f},{0.715106f,0.574073f},{0.722723f,0.574073f},
{0.285129f,0.527934f},{0.296555f,0.248643f},{0.285129f,0.248643f},
{0.285129f,0.527934f},{0.296555f,0.527934f},{0.296555f,0.248643f},
{0.296555f,0.527934f},{0.285129f,0.248643f},{0.292746f,0.248643f},
{0.462826f,0.527934f},{0.474253f,0.248643f},{0.462826f,0.248643f},
{0.462826f,0.527934f},{0.474252f,0.527934f},{0.474253f,0.248643f},
{0.474252f,0.527934f},{0.470444f,0.248643f},{0.206191f,0.012792f},
{0.320585f,0.017893f},{0.600932f,1.34113f},{0.38366f,1.34113f},
{0.340565f,0.928481f},{0.644027f,0.928482f},{0.340565f,0.94118f},
{0.644027f,0.941181f},{0.644027f,0.928482f},{0.340565f,0.928481f},
{0.5f,1.0f},{0.083333f,1.0f},{-0.25f,1.0f},
{0.416667f,1.0f},{0.75f,1.0f},{0.083333f,1.0f},
{0.206191f,0.012792f},{0.206191f,0.012792f},{0.326575f,0.013401f},
{0.326575f,0.013401f},{0.258937f,0.818404f},{0.499123f,0.818404f},
{0.450942f,0.677332f},{0.307118f,0.677332f},{0.307118f,0.304879f},
{0.450942f,0.30488f},{0.450393f,0.634335f},{0.305407f,0.634334f},
{0.305407f,0.678323f},{0.450393f,0.678323f},{0.499123f,0.818404f},
{0.258937f,0.818404f},{0.450942f,0.677332f},{0.307118f,0.677332f}
};

/////////////////////////////////////////////
// Function to stitch the triangles together
// and draw the ship
void DrawShip(void)
    {
    int iFace, iPoint;
    
    glBegin(GL_TRIANGLES);
        for(iFace = 0; iFace < 156; iFace++)
            for(iPoint = 0; iPoint < 3; iPoint++)
                {
                //glTexCoord2fv(textures[face_indicies[iFace][iPoint+6]]);
                glNormal3fv(normals[face_indicies[iFace][iPoint+3]]);
                glVertex3fv(vertices[face_indicies[iFace][iPoint]]);
                }
    glEnd();
    }

