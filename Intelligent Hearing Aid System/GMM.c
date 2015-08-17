#include "GMM.h"
#include <android/log.h>

int* dec;
const static int nClass = 3;
const static int featLen = 13;
const static int nStates = 2;
const static float meanFeat[13] = {-50.42043793257951,20.82551005252487,-2.017278949549064,9.638085845061134,-3.638200772042818,5.352442475101151,0.6748418519138228,0.120956497974079,1.87590483374293,0.1195364485475406,-0.6791197893243779,1.578398196768109,-1.30574273444048};
const static float varFeat[13] = {76.63374978290808,15.584577132299,16.83737569636543,11.6121085185689,21.62161676999831,13.03926426494798,23.54529903235283,17.07381383308204,22.97752899801186,22.37794434881939,17.27021633330197,17.13459805244256,18.98272221813583};

// mu[nStates][nClass][featLen]
const static float mu[2][3][13] = {
        {
                {-0.0315427788982371,-0.2775065302463521,-0.02310548812086023,0.1627169707926113,-0.1049759895018529,-0.009872485845522047,-0.0238964263012325,-0.04030838885080783,-0.07080790107150946,-0.04891815712212476,-0.0239959704063341,0.02036786301403049,0.00963149353131219},
				{0.08601686651010151,-0.02841310343882827,-0.2238475419518318,-0.152540411404648,-0.1046326597989096,-0.05999829279890216,-0.1839301914133532,-0.1543642065199298,-0.148326459604386,-0.140284762113236,-0.06458570817612999,-0.02904680733944002,-0.05326565404376315},
				{0.05695496169371377,0.2015867737805162,0.1461169799580846,0.1078872742644643,-0.06251353539684396,0.1283842916507967,0.1206341552151942,0.1526876265235257,0.09780505780061677,0.08650782813213176,0.03413555809767933,-0.04527839020481951,-0.03184846200985524}
        },{
                {-0.21016734,-0.30150195,0.17441019,-0.10471905,0.089827408,-0.20748829,0.062212271,0.11171203,0.062879672,0.017437544,-0.022007714,-0.019672808,-0.0069787897},
                {0.10202131,-0.1054222,-0.2815123,-0.15266926,-0.12779698,-0.066663056,-0.19054451,-0.19157397,-0.16167431,-0.10291793,-0.13427806,-0.026270518,-0.034729273},
                {-0.055180262,0.2037961,0.16540579,0.1336161,0.21912311,0.11201945,0.16530341,0.08308645,0.16287331,0.15041189,0.11202212,0.074858182,0.10864998}
        }};
// priors[nStates][nClass]
const static float prior[2][3] = {
		{0.4789092797959268,0.7734833036412225,0.4395662875483675},
		{0.5210907202040731,0.2265166963587774,0.5604337124516325}
		};

// sigma inverse [nStates][nClass][featLen][featLen]
const static float iSigma[2][3][13][13] = {
        {
        {{2037.171,30.035843,-18.756586,-66.264671,187.11368,-96.280819,185.98382,-102.56042,116.86191,-78.165607,43.526731,-21.70274,18.315378},
        {30.035843,137.47457,-52.431917,11.298636,-17.973224,24.982411,-45.42502,24.551037,-17.262279,7.7344051,0.15143036,-0.013113021,0.82668415},
        {-18.756586,-52.431917,170.29517,-34.472473,5.8224387,-19.537674,33.123061,-16.940841,11.890948,-3.7802003,-0.54110353,0.41124977,-0.62953539},
        {-66.264671,11.298636,-34.472473,35.016273,-14.988536,15.252564,-23.980475,3.3490719,3.1983529,2.4172521,-3.0973731,1.739258,1.0949183},
        {187.11368,-17.973224,5.8224387,-14.988536,93.553474,-46.666672,58.123364,-5.4543859,-4.9321895,-4.4785726,12.457787,-5.6128858,2.6037305},
        {-96.280819,24.982411,-19.537674,15.252564,-46.666672,49.95005,-53.598829,14.768462,-7.1287017,4.9900405,-4.7006313,4.8258112,-2.4790394},
        {185.98382,-45.42502,33.123061,-23.980475,58.123364,-53.598829,147.8053,-56.645364,36.937466,-11.904616,1.7100212,-2.4457091,4.3256966},
        {-102.56042,24.551037,-16.940841,3.3490719,-5.4543859,14.768462,-56.645364,70.992259,-54.154843,21.406258,-6.2012749,4.604051,-2.6694785},
        {116.86191,-17.262279,11.890948,3.1983529,-4.9321895,-7.1287017,36.937466,-54.154843,116.09062,-66.802017,23.106855,-8.5548792,7.9151063},
        {-78.165607,7.7344051,-3.7802003,2.4172521,-4.4785726,4.9900405,-11.904616,21.406258,-66.802017,101.25516,-41.493242,15.734941,-7.2846815},
        {43.526731,0.15143036,-0.54110353,-3.0973731,12.457787,-4.7006313,1.7100212,-6.2012749,23.106855,-41.493242,51.945662,-27.012329,13.636862},
        {-21.70274,-0.013113021,0.41124977,1.739258,-5.6128858,4.8258112,-2.4457091,4.604051,-8.5548792,15.734941,-27.012329,44.148096,-22.754043},
        {18.315378,0.82668415,-0.62953539,1.0949183,2.6037305,-2.4790394,4.3256966,-2.6694785,7.9151063,-7.2846815,13.636862,-22.754043,38.61545}},

        {{688.0577,-36.84003,78.78,-65.208322,129.29079,-59.039656,108.91836,-50.525198,38.730073,-24.166495,9.3203503,-11.224732,5.2282905},
        {-36.84003,54.998989,-10.961151,21.271534,-34.820807,20.940982,-20.832525,10.283469,-8.293531,11.057805,-0.45423546,0.66287106,2.5771998},
        {78.78,-10.961151,70.483281,-29.472919,42.568811,-15.357917,19.300913,-8.4781506,9.8260276,-11.268183,6.9616536,-3.641716,0.65544414},
        {-65.208322,21.271534,-29.472919,32.904542,-34.033466,11.083115,-13.687381,6.1803476,-4.6030316,4.5501585,-0.94245556,1.701123,-0.41547867},
        {129.29079,-34.820807,42.568811,-34.033466,94.788402,-35.039859,41.512693,-12.533721,9.910238,-4.063577,3.6023101,-0.52707999,-0.26312905},
        {-59.039656,20.940982,-15.357917,11.083115,-35.039859,32.620956,-36.368939,14.643693,-5.7807933,3.988286,-0.76298743,0.21184105,1.6354132},
        {108.91836,-20.832525,19.300913,-13.687381,41.512693,-36.368939,98.767002,-40.609767,27.64801,-15.587186,3.936989,-1.196646,-1.1782186},
        {-50.525198,10.283469,-8.4781506,6.1803476,-12.533721,14.643693,-40.609767,47.91922,-35.927379,20.828737,-5.4381194,3.8944097,-0.23487938},
        {38.730073,-8.293531,9.8260276,-4.6030316,9.910238,-5.7807933,27.64801,-35.927379,77.127881,-44.753686,17.680366,-7.7236568,2.4414775},
        {-24.166495,11.057805,-11.268183,4.5501585,-4.063577,3.988286,-15.587186,20.828737,-44.753686,65.849232,-29.734378,16.963899,-6.5875363},
        {9.3203503,-0.45423546,6.9616536,-0.94245556,3.6023101,-0.76298743,3.936989,-5.4381194,17.680366,-29.734378,37.153006,-21.78289,11.349053},
        {-11.224732,0.66287106,-3.641716,1.701123,-0.52707999,0.21184105,-1.196646,3.8944097,-7.7236568,16.963899,-21.78289,33.210938,-15.830237},
        {5.2282905,2.5771998,0.65544414,-0.41547867,-0.26312905,1.6354132,-1.1782186,-0.23487938,2.4414775,-6.5875363,11.349053,-15.830237,25.820332}},

        {{446.06156,-31.342322,84.470252,-43.720806,113.22606,-55.493138,89.991588,-45.553696,43.725681,-51.781701,32.377878,-20.179015,10.657848},
        {-31.342322,51.441211,-7.4438909,7.4314973,-2.1404473,5.6931665,-20.797488,17.731466,-24.720308,19.711529,-11.718469,6.2919196,-2.3639308},
        {84.470252,-7.4438909,54.792396,-14.243019,5.2193896,0.5880987,4.0139579,-10.205622,16.730303,-14.575521,8.5880395,-1.8651774,2.7323761},
        {-43.720806,7.4314973,-14.243019,21.587328,-17.068753,-1.2961007,1.3126432,2.9849063,-4.6009321,8.4286879,-6.3784815,3.3557632,-2.2926923},
        {113.22606,-2.1404473,5.2193896,-17.068753,87.84598,-23.670804,10.528886,-3.3491182,7.3758195,-11.04202,11.829048,-9.2188244,2.8554292},
        {-55.493138,5.6931665,0.5880987,-1.2961007,-23.670804,33.217058,-29.541269,7.0915084,-0.91560545,5.4560177,-5.301961,2.8743601,-1.622456},
        {89.991588,-20.797488,4.0139579,1.3126432,10.528886,-29.541269,106.71748,-39.119609,16.732374,-7.011806,5.391354,-1.8487632,0.75285044},
        {-45.553696,17.731466,-10.205622,2.9849063,-3.3491182,7.0915084,-39.119609,49.963638,-32.428047,13.508973,-6.2405407,2.8015572,-1.4504032},
        {43.725681,-24.720308,16.730303,-4.6009321,7.3758195,-0.91560545,16.732374,-32.428047,80.094327,-46.499585,15.831942,-7.1857685,2.9262584},
        {-51.781701,19.711529,-14.575521,8.4286879,-11.04202,5.4560177,-7.011806,13.508973,-46.499585,77.569126,-31.347966,9.0565664,-2.4671815},
        {32.377878,-11.718469,8.5880395,-6.3784815,11.829048,-5.301961,5.391354,-6.2405407,15.831942,-31.347966,39.732605,-18.907904,5.868143},
        {-20.179015,6.2919196,-1.8651774,3.3557632,-9.2188244,2.8743601,-1.8487632,2.8015572,-7.1857685,9.0565664,-18.907904,33.66726,-15.351533},
        {10.657848,-2.3639308,2.7323761,-2.2926923,2.8554292,-1.622456,0.75285044,-1.4504032,2.9262584,-2.4671815,5.868143,-15.351533,29.866161}}

        },{

        {{5467.6311,-1570.9552,1174.5766,-561.11468,739.92247,-275.89316,255.41738,-121.18296,101.30411,-9.8733451,-24.398562,47.627562,-8.6662325},
        {-1570.9552,623.34861,-345.58429,139.31335,-157.25531,71.906829,-86.998765,38.564,-28.46654,7.6802393,-0.57730123,-5.1868286,-1.4598402},
        {1174.5766,-345.58429,321.68297,-120.844,108.90709,-43.793413,59.672176,-27.157046,18.86942,-8.6743572,6.4926105,-1.7033311,4.1815343},
        {-561.11468,139.31335,-120.844,91.703971,-88.926768,25.428046,-28.038467,13.397269,-9.8570974,8.5254132,-4.9979912,0.59139129,-1.5357629},
        {739.92247,-157.25531,108.90709,-88.926768,214.95677,-72.773916,59.429154,-20.496536,17.545931,-14.006116,9.7223121,-3.7753549,1.7455898},
        {-275.89316,71.906829,-43.793413,25.428046,-72.773916,64.960599,-61.714469,18.292722,-10.887686,6.502685,-3.5411662,2.7647423,-2.0331334},
        {255.41738,-86.998765,59.672176,-28.038467,59.429154,-61.714469,164.14191,-64.079509,35.379445,-16.41574,10.387986,-7.8689418,3.4103336},
        {-121.18296,38.564,-27.157046,13.397269,-20.496536,18.292722,-64.079509,74.974366,-58.155447,23.837066,-9.6493553,5.83756,-3.8436883},
        {101.30411,-28.46654,18.86942,-9.8570974,17.545931,-10.887686,35.379445,-58.155447,127.35546,-72.671398,24.870725,-10.866807,6.3126852},
        {-9.8733451,7.6802393,-8.6743572,8.5254132,-14.006116,6.502685,-16.41574,23.837066,-72.671398,107.57677,-48.891364,22.582465,-8.6967486},
        {-24.398562,-0.57730123,6.4926105,-4.9979912,9.7223121,-3.5411662,10.387986,-9.6493553,24.870725,-48.891364,58.653977,-34.257019,12.926373},
        {47.627562,-5.1868286,-1.7033311,0.59139129,-3.7753549,2.7647423,-7.8689418,5.83756,-10.866807,22.582465,-34.257019,53.220905,-26.260377},
        {-8.6662325,-1.4598402,4.1815343,-1.5357629,1.7455898,-2.0331334,3.4103336,-3.8436883,6.3126852,-8.6967486,12.926373,-26.260377,38.447977}},

        {{415.75271,37.690998,27.899976,4.3865712,16.409771,6.9802623,24.017894,-10.91543,22.241217,-5.7818437,10.544169,1.5459321,10.988099},
        {37.690998,38.024292,9.2323102,6.3427609,-5.1943065,4.804816,-1.1477131,5.5955171,-4.5079938,4.7934637,2.0669728,1.0272166,1.194278},
        {27.899976,9.2323102,27.777328,1.2290228,1.2887492,0.1938123,6.2328764,-4.184045,4.8952186,-1.3695444,2.5377435,-0.71046199,4.0581784},
        {4.3865712,6.3427609,1.2290228,10.989475,-2.8900687,2.0568012,-5.8503784,3.0709938,-2.1202303,2.7561629,-1.3202271,1.9187055,-0.57955119},
        {16.409771,-5.1943065,1.2887492,-2.8900687,36.149686,-9.6058068,6.6726948,-1.1356461,3.5997984,-2.3682966,3.2511193,-1.7858912,2.416348},
        {6.9802623,4.804816,0.1938123,2.0568012,-9.6058068,12.469066,-6.0683928,2.8079107,-0.44877915,0.37289582,-0.36582998,1.8199067,-1.0982814},
        {24.017894,-1.1477131,6.2328764,-5.8503784,6.6726948,-6.0683928,39.565389,-10.387917,4.5934791,-6.4728275,2.3882607,-2.9236164,1.815742},
        {-10.91543,5.5955171,-4.184045,3.0709938,-1.1356461,2.8079107,-10.387917,20.269325,-10.483991,6.5330579,-1.2669148,0.89473136,1.257405},
        {22.241217,-4.5079938,4.8952186,-2.1202303,3.5997984,-0.44877915,4.5934791,-10.483991,40.457065,-16.310439,5.1233676,0.14994101,1.0691718},
        {-5.7818437,4.7934637,-1.3695444,2.7561629,-2.3682966,0.37289582,-6.4728275,6.5330579,-16.310439,35.543739,-11.268328,4.9451647,-1.9398989},
        {10.544169,2.0669728,2.5377435,-1.3202271,3.2511193,-0.36582998,2.3882607,-1.2669148,5.1233676,-11.268328,22.826235,-8.2983656,4.2362595},
        {1.5459321,1.0272166,-0.71046199,1.9187055,-1.7858912,1.8199067,-2.9236164,0.89473136,0.14994101,4.9451647,-8.2983656,18.912536,-6.1733953},
        {10.988099,1.194278,4.0581784,-0.57955119,2.416348,-1.0982814,1.815742,1.257405,1.0691718,-1.9398989,4.2362595,-6.1733953,17.011282}},

        {{5105.1886,-1316.0779,737.75241,-431.8228,735.15705,-332.49712,374.69053,-171.95163,134.7579,-22.096331,12.448915,28.876164,-3.771969},
        {-1316.0779,402.34297,-244.35007,137.37508,-209.5018,88.51202,-94.824151,41.331788,-35.324369,2.2464912,-0.58351689,-3.9040595,0.9855169},
        {737.75241,-244.35007,275.05325,-117.82354,147.59244,-47.174595,45.528902,-25.867862,25.780283,-8.7064662,4.3350196,-1.7878492,2.6310669},
        {-431.8228,137.37508,-117.82354,83.191933,-89.644177,21.370312,-26.723242,10.877237,-9.3333907,7.1770728,-4.6862972,0.95724799,0.33086752},
        {735.15705,-209.5018,147.59244,-89.644177,179.41456,-68.534088,70.966556,-25.05557,22.775248,-19.898268,7.8812668,-6.2614282,-8.097692},
        {-332.49712,88.51202,-47.174595,21.370312,-68.534088,61.342569,-64.577625,20.877352,-14.430011,10.033978,-5.7388444,3.2298933,-0.60069575},
        {374.69053,-94.824151,45.528902,-26.723242,70.966556,-64.577625,162.92808,-73.037144,47.689781,-22.993,13.746522,-8.9166697,7.6221258},
        {-171.95163,41.331788,-25.867862,10.877237,-25.05557,20.877352,-73.037144,77.997547,-63.616593,29.000788,-13.263407,7.362621,-4.5307744},
        {134.7579,-35.324369,25.780283,-9.3333907,22.775248,-14.430011,47.689781,-63.616593,129.29845,-75.484375,28.435202,-11.288138,6.4299477},
        {-22.096331,2.2464912,-8.7064662,7.1770728,-19.898268,10.033978,-22.993,29.000788,-75.484375,107.4583,-50.981368,21.087583,-9.6177766},
        {12.448915,-0.58351689,4.3350196,-4.6862972,7.8812668,-5.7388444,13.746522,-13.263407,28.435202,-50.981368,61.189905,-33.012083,13.532996},
        {28.876164,-3.9040595,-1.7878492,0.95724799,-6.2614282,3.2298933,-8.9166697,7.362621,-11.288138,21.087583,-33.012083,48.348119,-24.933456},
        {-3.771969,0.9855169,2.6310669,0.33086752,-8.097692,-0.60069575,7.6221258,-4.5307744,6.4299477,-9.6177766,13.532996,-24.933456,37.707568}}
        }};

// MulConst[nSates][nClass]
const static float mulConst[2][3] = {
		{5010175.124509772,154489.5855781778,182042.8397230803},
		{9898478.685644863,9149.217393356368,4889265.238517599}
		};
int windowsCount = 0;//here
float maxProb;
int maxProbInd;
int nbuf = 20; //here   //to get as input from the app
int decBufInd = 0;//here
int decAvailable = 0;//here
int classDec = -1;//here
int maxCount;
int decBuf[20] = {0};//here
int decBufCountClass[] = {20,0,0,0};//here  //DecBufCountClass[0] = Nbuf

int computeProb(float* mfccout)
{
	dec = (int*) calloc(2, sizeof(int));
	int i,j,k,l;
	float xSig;
	float p,t;
	float x[featLen];
	float x_mu[featLen];
	float totalProb[nClass];

	for(i = 0; i<featLen; i++)
	{
		x[i] = (float) (mfccout[i] - meanFeat[i])/varFeat[i];
		__android_log_print(ANDROID_LOG_INFO, "mytag","outside mfcc buffer finish");
	}

	for(i = 0 ; i < nClass ; i++)
	{
		totalProb[i] = 0.0;
		for(j = 0 ; j < nStates ; j++)
		{
			for(k = 0 ; k < featLen ; k++)
			{
				x_mu[k] =  (x[k] - (float) mu[j][i][k]);
			}
			p = 0.0;
			for(k = 0 ; k < featLen ; k++)
			{
				xSig = 0.0;
				for(l = 0 ; l < featLen ; l++)
				{
					xSig = (float)  xSig + (x_mu[l] * (float) iSigma[j][i][k][l]);
				}
				p = p + (xSig * x_mu[k]);
			}
			p =  p * -0.5;
			p =  (float) (exp(p)* (float) mulConst[j][i]);
			totalProb[i] = (float) (totalProb[i] + (p* prior[j][i]));
		}


		if (i==0)
		{
			maxProb = totalProb[0];
			maxProbInd = 0;
		}
		else
		{
			if(totalProb[i] > maxProb)
			{
				maxProb = totalProb[i];
				maxProbInd = i;
			}
		}
	}

	decBufCountClass[decBuf[decBufInd]]--;
	decBuf[decBufInd++] = maxProbInd;

	if(decBufInd>=nbuf)
	decBufInd = 0;

	decBufCountClass[maxProbInd]++;

	if(decAvailable)
	{
		classDec = 0;
		maxCount = decBufCountClass[0];
		for(j=1;j<nClass;j++)
		{
			if(decBufCountClass[j] > maxCount)
			{
				maxCount = decBufCountClass[j];
				classDec = j;
			}
		}
	}
	else
	{
		windowsCount++;
		if(windowsCount > nbuf)
		decAvailable = 1;
	}

return classDec;
}