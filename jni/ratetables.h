#ifndef _ratetables_h_
#define _ratetables_h_

//预先生成的形变衰减率
static const int RateTablesCount = 50;
static const float RateTables[RateTablesCount][RateTablesCount] = {
    { 0.7000, 0.6993, 0.6972, 0.6938, 0.6890, 0.6829, 0.6754, 0.6667, 0.6567, 0.6455, 0.6332, 0.6197, 0.6051, 0.5896, 0.5731, 0.5557, 0.5375, 0.5186, 0.4990, 0.4788, 0.4582, 0.4370, 0.4156, 0.3939, 0.3720, 0.3500, 0.3280, 0.3061, 0.2844, 0.2630, 0.2418, 0.2212, 0.2010, 0.1814, 0.1625, 0.1443, 0.1269, 0.1104, 0.0949, 0.0803, 0.0668, 0.0545, 0.0433, 0.0333, 0.0246, 0.0171, 0.0110, 0.0062, 0.0028, 0.0007},
    { 0.6993, 0.6986, 0.6966, 0.6931, 0.6883, 0.6822, 0.6747, 0.6660, 0.6560, 0.6449, 0.6325, 0.6190, 0.6045, 0.5890, 0.5725, 0.5551, 0.5370, 0.5180, 0.4985, 0.4783, 0.4576, 0.4365, 0.4151, 0.3934, 0.3715, 0.3496, 0.3276, 0.3057, 0.2840, 0.2626, 0.2415, 0.2208, 0.2007, 0.1811, 0.1622, 0.1440, 0.1267, 0.1102, 0.0947, 0.0801, 0.0667, 0.0543, 0.0432, 0.0332, 0.0245, 0.0171, 0.0109, 0.0062, 0.0027, 0.0007},
    { 0.6972, 0.6966, 0.6945, 0.6911, 0.6863, 0.6802, 0.6727, 0.6640, 0.6541, 0.6429, 0.6306, 0.6171, 0.6026, 0.5871, 0.5707, 0.5534, 0.5352, 0.5163, 0.4968, 0.4767, 0.4561, 0.4350, 0.4136, 0.3920, 0.3702, 0.3482, 0.3263, 0.3045, 0.2829, 0.2615, 0.2405, 0.2198, 0.1997, 0.1802, 0.1614, 0.1433, 0.1260, 0.1095, 0.0941, 0.0796, 0.0662, 0.0539, 0.0428, 0.0329, 0.0242, 0.0168, 0.0108, 0.0060, 0.0026, 0.0006},
    { 0.6938, 0.6931, 0.6911, 0.6876, 0.6829, 0.6768, 0.6694, 0.6607, 0.6508, 0.6396, 0.6274, 0.6140, 0.5995, 0.5841, 0.5677, 0.5504, 0.5323, 0.5135, 0.4941, 0.4740, 0.4535, 0.4325, 0.4112, 0.3896, 0.3679, 0.3461, 0.3242, 0.3025, 0.2810, 0.2597, 0.2387, 0.2182, 0.1982, 0.1788, 0.1600, 0.1420, 0.1248, 0.1085, 0.0931, 0.0787, 0.0654, 0.0532, 0.0422, 0.0323, 0.0238, 0.0165, 0.0105, 0.0058, 0.0025, 0.0006},
    { 0.6890, 0.6883, 0.6863, 0.6829, 0.6781, 0.6721, 0.6647, 0.6560, 0.6462, 0.6351, 0.6229, 0.6095, 0.5952, 0.5798, 0.5635, 0.5463, 0.5283, 0.5096, 0.4902, 0.4703, 0.4498, 0.4290, 0.4078, 0.3863, 0.3647, 0.3430, 0.3213, 0.2997, 0.2783, 0.2571, 0.2363, 0.2159, 0.1960, 0.1768, 0.1581, 0.1402, 0.1232, 0.1070, 0.0917, 0.0775, 0.0643, 0.0522, 0.0413, 0.0316, 0.0231, 0.0159, 0.0101, 0.0055, 0.0023, 0.0005},
    { 0.6829, 0.6822, 0.6802, 0.6768, 0.6721, 0.6660, 0.6587, 0.6501, 0.6403, 0.6293, 0.6171, 0.6039, 0.5896, 0.5743, 0.5581, 0.5410, 0.5232, 0.5046, 0.4853, 0.4655, 0.4452, 0.4245, 0.4034, 0.3821, 0.3607, 0.3391, 0.3176, 0.2961, 0.2749, 0.2539, 0.2332, 0.2130, 0.1933, 0.1742, 0.1557, 0.1380, 0.1211, 0.1051, 0.0900, 0.0759, 0.0629, 0.0510, 0.0402, 0.0307, 0.0223, 0.0153, 0.0096, 0.0052, 0.0021, 0.0004},
    { 0.6754, 0.6747, 0.6727, 0.6694, 0.6647, 0.6587, 0.6514, 0.6429, 0.6332, 0.6222, 0.6102, 0.5970, 0.5828, 0.5677, 0.5516, 0.5346, 0.5169, 0.4985, 0.4794, 0.4597, 0.4396, 0.4190, 0.3982, 0.3770, 0.3557, 0.3344, 0.3131, 0.2918, 0.2707, 0.2499, 0.2295, 0.2095, 0.1900, 0.1711, 0.1528, 0.1353, 0.1186, 0.1028, 0.0879, 0.0740, 0.0612, 0.0495, 0.0389, 0.0295, 0.0214, 0.0145, 0.0090, 0.0047, 0.0018, 0.0003},
    { 0.6667, 0.6660, 0.6640, 0.6607, 0.6560, 0.6501, 0.6429, 0.6344, 0.6248, 0.6140, 0.6020, 0.5890, 0.5749, 0.5599, 0.5439, 0.5272, 0.5096, 0.4913, 0.4724, 0.4529, 0.4330, 0.4126, 0.3920, 0.3711, 0.3500, 0.3289, 0.3078, 0.2867, 0.2659, 0.2453, 0.2251, 0.2054, 0.1861, 0.1674, 0.1494, 0.1321, 0.1157, 0.1001, 0.0854, 0.0718, 0.0592, 0.0477, 0.0374, 0.0282, 0.0203, 0.0136, 0.0083, 0.0042, 0.0015, 0.0002},
    { 0.6567, 0.6560, 0.6541, 0.6508, 0.6462, 0.6403, 0.6332, 0.6248, 0.6152, 0.6045, 0.5927, 0.5798, 0.5659, 0.5510, 0.5352, 0.5186, 0.5012, 0.4832, 0.4645, 0.4452, 0.4255, 0.4054, 0.3849, 0.3643, 0.3434, 0.3226, 0.3017, 0.2810, 0.2604, 0.2401, 0.2202, 0.2007, 0.1817, 0.1633, 0.1455, 0.1286, 0.1124, 0.0971, 0.0827, 0.0693, 0.0570, 0.0457, 0.0356, 0.0267, 0.0191, 0.0127, 0.0075, 0.0037, 0.0012, 0.0001},
    { 0.6455, 0.6449, 0.6429, 0.6396, 0.6351, 0.6293, 0.6222, 0.6140, 0.6045, 0.5939, 0.5822, 0.5695, 0.5557, 0.5410, 0.5254, 0.5090, 0.4919, 0.4740, 0.4555, 0.4365, 0.4171, 0.3972, 0.3770, 0.3566, 0.3361, 0.3155, 0.2950, 0.2745, 0.2542, 0.2343, 0.2146, 0.1954, 0.1768, 0.1587, 0.1412, 0.1246, 0.1087, 0.0937, 0.0796, 0.0665, 0.0545, 0.0435, 0.0337, 0.0251, 0.0177, 0.0116, 0.0067, 0.0032, 0.0009, 0.0000},
    { 0.6332, 0.6325, 0.6306, 0.6274, 0.6229, 0.6171, 0.6102, 0.6020, 0.5927, 0.5822, 0.5707, 0.5581, 0.5445, 0.5300, 0.5147, 0.4985, 0.4815, 0.4639, 0.4457, 0.4270, 0.4078, 0.3882, 0.3683, 0.3482, 0.3280, 0.3078, 0.2875, 0.2674, 0.2475, 0.2278, 0.2085, 0.1897, 0.1713, 0.1536, 0.1365, 0.1202, 0.1047, 0.0900, 0.0762, 0.0635, 0.0518, 0.0412, 0.0317, 0.0234, 0.0163, 0.0105, 0.0059, 0.0026, 0.0006, 0.0000},
    { 0.6197, 0.6190, 0.6171, 0.6140, 0.6095, 0.6039, 0.5970, 0.5890, 0.5798, 0.5695, 0.5581, 0.5457, 0.5323, 0.5180, 0.5029, 0.4870, 0.4703, 0.4529, 0.4350, 0.4166, 0.3977, 0.3784, 0.3589, 0.3391, 0.3192, 0.2993, 0.2794, 0.2597, 0.2401, 0.2208, 0.2019, 0.1834, 0.1655, 0.1481, 0.1314, 0.1155, 0.1003, 0.0860, 0.0726, 0.0603, 0.0489, 0.0386, 0.0295, 0.0216, 0.0148, 0.0093, 0.0050, 0.0021, 0.0004, 0.0000},
    { 0.6051, 0.6045, 0.6026, 0.5995, 0.5952, 0.5896, 0.5828, 0.5749, 0.5659, 0.5557, 0.5445, 0.5323, 0.5192, 0.5051, 0.4902, 0.4746, 0.4582, 0.4411, 0.4235, 0.4054, 0.3868, 0.3679, 0.3487, 0.3293, 0.3098, 0.2902, 0.2707, 0.2514, 0.2322, 0.2133, 0.1948, 0.1768, 0.1592, 0.1423, 0.1260, 0.1104, 0.0957, 0.0818, 0.0688, 0.0568, 0.0459, 0.0360, 0.0272, 0.0196, 0.0132, 0.0081, 0.0042, 0.0015, 0.0002, 0.0000},
    { 0.5896, 0.5890, 0.5871, 0.5841, 0.5798, 0.5743, 0.5677, 0.5599, 0.5510, 0.5410, 0.5300, 0.5180, 0.5051, 0.4913, 0.4767, 0.4613, 0.4452, 0.4285, 0.4112, 0.3934, 0.3752, 0.3566, 0.3378, 0.3188, 0.2997, 0.2806, 0.2615, 0.2425, 0.2238, 0.2054, 0.1873, 0.1697, 0.1525, 0.1360, 0.1202, 0.1051, 0.0908, 0.0773, 0.0648, 0.0532, 0.0427, 0.0332, 0.0249, 0.0177, 0.0117, 0.0069, 0.0033, 0.0011, 0.0001, 0.0000},
    { 0.5731, 0.5725, 0.5707, 0.5677, 0.5635, 0.5581, 0.5516, 0.5439, 0.5352, 0.5254, 0.5147, 0.5029, 0.4902, 0.4767, 0.4624, 0.4473, 0.4315, 0.4151, 0.3982, 0.3807, 0.3629, 0.3447, 0.3263, 0.3078, 0.2891, 0.2704, 0.2517, 0.2332, 0.2149, 0.1970, 0.1793, 0.1622, 0.1455, 0.1295, 0.1141, 0.0995, 0.0856, 0.0726, 0.0606, 0.0495, 0.0394, 0.0303, 0.0224, 0.0157, 0.0101, 0.0057, 0.0025, 0.0006, -0.0000, 0.0000},
    { 0.5557, 0.5551, 0.5534, 0.5504, 0.5463, 0.5410, 0.5346, 0.5272, 0.5186, 0.5090, 0.4985, 0.4870, 0.4746, 0.4613, 0.4473, 0.4325, 0.4171, 0.4010, 0.3845, 0.3674, 0.3500, 0.3323, 0.3143, 0.2961, 0.2779, 0.2597, 0.2415, 0.2235, 0.2057, 0.1882, 0.1711, 0.1544, 0.1383, 0.1227, 0.1078, 0.0937, 0.0803, 0.0678, 0.0562, 0.0456, 0.0360, 0.0274, 0.0200, 0.0136, 0.0085, 0.0045, 0.0018, 0.0003, 0.0000, 0.0000},
    { 0.5375, 0.5370, 0.5352, 0.5323, 0.5283, 0.5232, 0.5169, 0.5096, 0.5012, 0.4919, 0.4815, 0.4703, 0.4582, 0.4452, 0.4315, 0.4171, 0.4020, 0.3863, 0.3702, 0.3535, 0.3365, 0.3192, 0.3017, 0.2840, 0.2663, 0.2485, 0.2308, 0.2133, 0.1960, 0.1791, 0.1625, 0.1463, 0.1307, 0.1157, 0.1013, 0.0877, 0.0749, 0.0629, 0.0518, 0.0417, 0.0326, 0.0245, 0.0175, 0.0117, 0.0070, 0.0035, 0.0012, 0.0001, 0.0000, 0.0000},
    { 0.5186, 0.5180, 0.5163, 0.5135, 0.5096, 0.5046, 0.4985, 0.4913, 0.4832, 0.4740, 0.4639, 0.4529, 0.4411, 0.4285, 0.4151, 0.4010, 0.3863, 0.3711, 0.3553, 0.3391, 0.3226, 0.3057, 0.2887, 0.2715, 0.2542, 0.2370, 0.2198, 0.2028, 0.1861, 0.1697, 0.1536, 0.1380, 0.1229, 0.1085, 0.0947, 0.0816, 0.0693, 0.0578, 0.0473, 0.0377, 0.0291, 0.0216, 0.0151, 0.0097, 0.0055, 0.0025, 0.0006, 0.0000, 0.0000, 0.0000},
    { 0.4990, 0.4985, 0.4968, 0.4941, 0.4902, 0.4853, 0.4794, 0.4724, 0.4645, 0.4555, 0.4457, 0.4350, 0.4235, 0.4112, 0.3982, 0.3845, 0.3702, 0.3553, 0.3400, 0.3242, 0.3082, 0.2918, 0.2753, 0.2586, 0.2418, 0.2251, 0.2085, 0.1921, 0.1759, 0.1600, 0.1445, 0.1295, 0.1150, 0.1011, 0.0879, 0.0754, 0.0637, 0.0528, 0.0428, 0.0337, 0.0257, 0.0187, 0.0127, 0.0079, 0.0042, 0.0016, 0.0003, 0.0000, 0.0000, 0.0000},
    { 0.4788, 0.4783, 0.4767, 0.4740, 0.4703, 0.4655, 0.4597, 0.4529, 0.4452, 0.4365, 0.4270, 0.4166, 0.4054, 0.3934, 0.3807, 0.3674, 0.3535, 0.3391, 0.3242, 0.3090, 0.2934, 0.2775, 0.2615, 0.2453, 0.2292, 0.2130, 0.1970, 0.1811, 0.1655, 0.1502, 0.1353, 0.1209, 0.1070, 0.0937, 0.0810, 0.0691, 0.0580, 0.0477, 0.0383, 0.0298, 0.0223, 0.0159, 0.0105, 0.0062, 0.0030, 0.0009, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.4582, 0.4576, 0.4561, 0.4535, 0.4498, 0.4452, 0.4396, 0.4330, 0.4255, 0.4171, 0.4078, 0.3977, 0.3868, 0.3752, 0.3629, 0.3500, 0.3365, 0.3226, 0.3082, 0.2934, 0.2783, 0.2630, 0.2475, 0.2319, 0.2162, 0.2007, 0.1852, 0.1699, 0.1549, 0.1402, 0.1260, 0.1121, 0.0989, 0.0862, 0.0742, 0.0629, 0.0524, 0.0427, 0.0339, 0.0260, 0.0191, 0.0132, 0.0083, 0.0046, 0.0019, 0.0004, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.4370, 0.4365, 0.4350, 0.4325, 0.4290, 0.4245, 0.4190, 0.4126, 0.4054, 0.3972, 0.3882, 0.3784, 0.3679, 0.3566, 0.3447, 0.3323, 0.3192, 0.3057, 0.2918, 0.2775, 0.2630, 0.2482, 0.2332, 0.2182, 0.2032, 0.1882, 0.1733, 0.1587, 0.1443, 0.1302, 0.1166, 0.1034, 0.0908, 0.0787, 0.0673, 0.0567, 0.0468, 0.0377, 0.0295, 0.0223, 0.0159, 0.0106, 0.0064, 0.0032, 0.0011, 0.0001, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.4156, 0.4151, 0.4136, 0.4112, 0.4078, 0.4034, 0.3982, 0.3920, 0.3849, 0.3770, 0.3683, 0.3589, 0.3487, 0.3378, 0.3263, 0.3143, 0.3017, 0.2887, 0.2753, 0.2615, 0.2475, 0.2332, 0.2189, 0.2044, 0.1900, 0.1756, 0.1614, 0.1473, 0.1336, 0.1202, 0.1072, 0.0947, 0.0827, 0.0713, 0.0606, 0.0505, 0.0413, 0.0329, 0.0253, 0.0187, 0.0130, 0.0083, 0.0046, 0.0020, 0.0004, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.3939, 0.3934, 0.3920, 0.3896, 0.3863, 0.3821, 0.3770, 0.3711, 0.3643, 0.3566, 0.3482, 0.3391, 0.3293, 0.3188, 0.3078, 0.2961, 0.2840, 0.2715, 0.2586, 0.2453, 0.2319, 0.2182, 0.2044, 0.1906, 0.1768, 0.1630, 0.1494, 0.1360, 0.1229, 0.1102, 0.0979, 0.0860, 0.0747, 0.0640, 0.0539, 0.0446, 0.0360, 0.0282, 0.0213, 0.0153, 0.0102, 0.0062, 0.0031, 0.0011, 0.0001, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.3720, 0.3715, 0.3702, 0.3679, 0.3647, 0.3607, 0.3557, 0.3500, 0.3434, 0.3361, 0.3280, 0.3192, 0.3098, 0.2997, 0.2891, 0.2779, 0.2663, 0.2542, 0.2418, 0.2292, 0.2162, 0.2032, 0.1900, 0.1768, 0.1636, 0.1505, 0.1375, 0.1248, 0.1124, 0.1003, 0.0886, 0.0775, 0.0668, 0.0568, 0.0474, 0.0388, 0.0309, 0.0238, 0.0175, 0.0122, 0.0077, 0.0043, 0.0018, 0.0004, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.3500, 0.3496, 0.3482, 0.3461, 0.3430, 0.3391, 0.3344, 0.3289, 0.3226, 0.3155, 0.3078, 0.2993, 0.2902, 0.2806, 0.2704, 0.2597, 0.2485, 0.2370, 0.2251, 0.2130, 0.2007, 0.1882, 0.1756, 0.1630, 0.1505, 0.1380, 0.1257, 0.1137, 0.1019, 0.0906, 0.0796, 0.0691, 0.0592, 0.0499, 0.0412, 0.0332, 0.0260, 0.0196, 0.0140, 0.0093, 0.0055, 0.0027, 0.0009, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.3280, 0.3276, 0.3263, 0.3242, 0.3213, 0.3176, 0.3131, 0.3078, 0.3017, 0.2950, 0.2875, 0.2794, 0.2707, 0.2615, 0.2517, 0.2415, 0.2308, 0.2198, 0.2085, 0.1970, 0.1852, 0.1733, 0.1614, 0.1494, 0.1375, 0.1257, 0.1141, 0.1028, 0.0917, 0.0810, 0.0708, 0.0610, 0.0518, 0.0432, 0.0352, 0.0279, 0.0214, 0.0157, 0.0108, 0.0067, 0.0036, 0.0015, 0.0003, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.3061, 0.3057, 0.3045, 0.3025, 0.2997, 0.2961, 0.2918, 0.2867, 0.2810, 0.2745, 0.2674, 0.2597, 0.2514, 0.2425, 0.2332, 0.2235, 0.2133, 0.2028, 0.1921, 0.1811, 0.1699, 0.1587, 0.1473, 0.1360, 0.1248, 0.1137, 0.1028, 0.0921, 0.0818, 0.0718, 0.0623, 0.0532, 0.0447, 0.0368, 0.0295, 0.0230, 0.0171, 0.0121, 0.0079, 0.0045, 0.0021, 0.0006, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.2844, 0.2840, 0.2829, 0.2810, 0.2783, 0.2749, 0.2707, 0.2659, 0.2604, 0.2542, 0.2475, 0.2401, 0.2322, 0.2238, 0.2149, 0.2057, 0.1960, 0.1861, 0.1759, 0.1655, 0.1549, 0.1443, 0.1336, 0.1229, 0.1124, 0.1019, 0.0917, 0.0818, 0.0721, 0.0629, 0.0541, 0.0457, 0.0379, 0.0308, 0.0242, 0.0184, 0.0132, 0.0089, 0.0054, 0.0027, 0.0010, 0.0001, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.2630, 0.2626, 0.2615, 0.2597, 0.2571, 0.2539, 0.2499, 0.2453, 0.2401, 0.2343, 0.2278, 0.2208, 0.2133, 0.2054, 0.1970, 0.1882, 0.1791, 0.1697, 0.1600, 0.1502, 0.1402, 0.1302, 0.1202, 0.1102, 0.1003, 0.0906, 0.0810, 0.0718, 0.0629, 0.0543, 0.0462, 0.0386, 0.0316, 0.0251, 0.0193, 0.0142, 0.0098, 0.0062, 0.0033, 0.0014, 0.0002, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.2418, 0.2415, 0.2405, 0.2387, 0.2363, 0.2332, 0.2295, 0.2251, 0.2202, 0.2146, 0.2085, 0.2019, 0.1948, 0.1873, 0.1793, 0.1711, 0.1625, 0.1536, 0.1445, 0.1353, 0.1260, 0.1166, 0.1072, 0.0979, 0.0886, 0.0796, 0.0708, 0.0623, 0.0541, 0.0462, 0.0389, 0.0320, 0.0257, 0.0200, 0.0149, 0.0105, 0.0068, 0.0039, 0.0017, 0.0004, -0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.2212, 0.2208, 0.2198, 0.2182, 0.2159, 0.2130, 0.2095, 0.2054, 0.2007, 0.1954, 0.1897, 0.1834, 0.1768, 0.1697, 0.1622, 0.1544, 0.1463, 0.1380, 0.1295, 0.1209, 0.1121, 0.1034, 0.0947, 0.0860, 0.0775, 0.0691, 0.0610, 0.0532, 0.0457, 0.0386, 0.0320, 0.0259, 0.0203, 0.0153, 0.0109, 0.0073, 0.0043, 0.0021, 0.0006, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.2010, 0.2007, 0.1997, 0.1982, 0.1960, 0.1933, 0.1900, 0.1861, 0.1817, 0.1768, 0.1713, 0.1655, 0.1592, 0.1525, 0.1455, 0.1383, 0.1307, 0.1229, 0.1150, 0.1070, 0.0989, 0.0908, 0.0827, 0.0747, 0.0668, 0.0592, 0.0518, 0.0447, 0.0379, 0.0316, 0.0257, 0.0203, 0.0154, 0.0112, 0.0075, 0.0046, 0.0023, 0.0008, 0.0001, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.1814, 0.1811, 0.1802, 0.1788, 0.1768, 0.1742, 0.1711, 0.1674, 0.1633, 0.1587, 0.1536, 0.1481, 0.1423, 0.1360, 0.1295, 0.1227, 0.1157, 0.1085, 0.1011, 0.0937, 0.0862, 0.0787, 0.0713, 0.0640, 0.0568, 0.0499, 0.0432, 0.0368, 0.0308, 0.0251, 0.0200, 0.0153, 0.0112, 0.0076, 0.0047, 0.0025, 0.0009, 0.0001, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.1625, 0.1622, 0.1614, 0.1600, 0.1581, 0.1557, 0.1528, 0.1494, 0.1455, 0.1412, 0.1365, 0.1314, 0.1260, 0.1202, 0.1141, 0.1078, 0.1013, 0.0947, 0.0879, 0.0810, 0.0742, 0.0673, 0.0606, 0.0539, 0.0474, 0.0412, 0.0352, 0.0295, 0.0242, 0.0193, 0.0149, 0.0109, 0.0075, 0.0047, 0.0025, 0.0010, 0.0002, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.1443, 0.1440, 0.1433, 0.1420, 0.1402, 0.1380, 0.1353, 0.1321, 0.1286, 0.1246, 0.1202, 0.1155, 0.1104, 0.1051, 0.0995, 0.0937, 0.0877, 0.0816, 0.0754, 0.0691, 0.0629, 0.0567, 0.0505, 0.0446, 0.0388, 0.0332, 0.0279, 0.0230, 0.0184, 0.0142, 0.0105, 0.0073, 0.0046, 0.0025, 0.0010, 0.0002, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.1269, 0.1267, 0.1260, 0.1248, 0.1232, 0.1211, 0.1186, 0.1157, 0.1124, 0.1087, 0.1047, 0.1003, 0.0957, 0.0908, 0.0856, 0.0803, 0.0749, 0.0693, 0.0637, 0.0580, 0.0524, 0.0468, 0.0413, 0.0360, 0.0309, 0.0260, 0.0214, 0.0171, 0.0132, 0.0098, 0.0068, 0.0043, 0.0023, 0.0009, 0.0002, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.1104, 0.1102, 0.1095, 0.1085, 0.1070, 0.1051, 0.1028, 0.1001, 0.0971, 0.0937, 0.0900, 0.0860, 0.0818, 0.0773, 0.0726, 0.0678, 0.0629, 0.0578, 0.0528, 0.0477, 0.0427, 0.0377, 0.0329, 0.0282, 0.0238, 0.0196, 0.0157, 0.0121, 0.0089, 0.0062, 0.0039, 0.0021, 0.0008, 0.0001, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.0949, 0.0947, 0.0941, 0.0931, 0.0917, 0.0900, 0.0879, 0.0854, 0.0827, 0.0796, 0.0762, 0.0726, 0.0688, 0.0648, 0.0606, 0.0562, 0.0518, 0.0473, 0.0428, 0.0383, 0.0339, 0.0295, 0.0253, 0.0213, 0.0175, 0.0140, 0.0108, 0.0079, 0.0054, 0.0033, 0.0017, 0.0006, 0.0001, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.0803, 0.0801, 0.0796, 0.0787, 0.0775, 0.0759, 0.0740, 0.0718, 0.0693, 0.0665, 0.0635, 0.0603, 0.0568, 0.0532, 0.0495, 0.0456, 0.0417, 0.0377, 0.0337, 0.0298, 0.0260, 0.0223, 0.0187, 0.0153, 0.0122, 0.0093, 0.0067, 0.0045, 0.0027, 0.0014, 0.0004, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.0668, 0.0667, 0.0662, 0.0654, 0.0643, 0.0629, 0.0612, 0.0592, 0.0570, 0.0545, 0.0518, 0.0489, 0.0459, 0.0427, 0.0394, 0.0360, 0.0326, 0.0291, 0.0257, 0.0223, 0.0191, 0.0159, 0.0130, 0.0102, 0.0077, 0.0055, 0.0036, 0.0021, 0.0010, 0.0002, -0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.0545, 0.0543, 0.0539, 0.0532, 0.0522, 0.0510, 0.0495, 0.0477, 0.0457, 0.0435, 0.0412, 0.0386, 0.0360, 0.0332, 0.0303, 0.0274, 0.0245, 0.0216, 0.0187, 0.0159, 0.0132, 0.0106, 0.0083, 0.0062, 0.0043, 0.0027, 0.0015, 0.0006, 0.0001, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.0433, 0.0432, 0.0428, 0.0422, 0.0413, 0.0402, 0.0389, 0.0374, 0.0356, 0.0337, 0.0317, 0.0295, 0.0272, 0.0249, 0.0224, 0.0200, 0.0175, 0.0151, 0.0127, 0.0105, 0.0083, 0.0064, 0.0046, 0.0031, 0.0018, 0.0009, 0.0003, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.0333, 0.0332, 0.0329, 0.0323, 0.0316, 0.0307, 0.0295, 0.0282, 0.0267, 0.0251, 0.0234, 0.0216, 0.0196, 0.0177, 0.0157, 0.0136, 0.0117, 0.0097, 0.0079, 0.0062, 0.0046, 0.0032, 0.0020, 0.0011, 0.0004, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.0246, 0.0245, 0.0242, 0.0238, 0.0231, 0.0223, 0.0214, 0.0203, 0.0191, 0.0177, 0.0163, 0.0148, 0.0132, 0.0117, 0.0101, 0.0085, 0.0070, 0.0055, 0.0042, 0.0030, 0.0019, 0.0011, 0.0004, 0.0001, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.0171, 0.0171, 0.0168, 0.0165, 0.0159, 0.0153, 0.0145, 0.0136, 0.0127, 0.0116, 0.0105, 0.0093, 0.0081, 0.0069, 0.0057, 0.0045, 0.0035, 0.0025, 0.0016, 0.0009, 0.0004, 0.0001, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.0110, 0.0109, 0.0108, 0.0105, 0.0101, 0.0096, 0.0090, 0.0083, 0.0075, 0.0067, 0.0059, 0.0050, 0.0042, 0.0033, 0.0025, 0.0018, 0.0012, 0.0006, 0.0003, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.0062, 0.0062, 0.0060, 0.0058, 0.0055, 0.0052, 0.0047, 0.0042, 0.0037, 0.0032, 0.0026, 0.0021, 0.0015, 0.0011, 0.0006, 0.0003, 0.0001, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.0028, 0.0027, 0.0026, 0.0025, 0.0023, 0.0021, 0.0018, 0.0015, 0.0012, 0.0009, 0.0006, 0.0004, 0.0002, 0.0001, -0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
    { 0.0007, 0.0007, 0.0006, 0.0006, 0.0005, 0.0004, 0.0003, 0.0002, 0.0001, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
};
#endif //_ratetables_h_
