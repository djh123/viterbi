// 一维和二维动态数组初始化为：


// std::vector <int> vec(10,90);  //将10个一维动态数组初始为90
// std::vector<std::vector<int> > vec(row,vector<int>(col,0));  //初始化row * col二维动态数组，初始化值为0

// 获取一维数组长度：

// int size = vec.size();

// 获取二维数组长度：
// int size_row = vec.size();  //获取行数
// int size_col = vec[0].size();  //获取列数

vector<int> viterbi_decode(vector<vector<float> > nodes, vector<vector<float> > trans){
    int seq_len = nodes.size();
    int num_labels = trans.size();
    vector<float> scores = nodes[0];
    vector<vector<int> > paths;
    for (int t = 1; t < seq_len; t++) {
        vector<vector<float> > scores_repeat;
        for (size_t i = 0; i < scores.size(); i++)
        {
            vector<float> scores_repeat_col;
            for (size_t j = 0; j < scores.size(); j++)
            {
                scores_repeat_col.push_back(scores[i]);
            }
            scores_repeat.push_back(scores_repeat_col);
        }
        vector<float> observe = nodes[t];
        vector<vector<float> > observe_repeat;
        for (size_t i = 0; i < observe.size(); i++)
        {
            observe_repeat.push_back(observe);
        }
        vector<vector<float> > M(scores_repeat.size(),vector<float>(scores_repeat[0].size()));
        for (size_t i = 0; i < scores_repeat.size(); i++)
        {
            for (size_t j = 0; j < scores_repeat[0].size(); j++)
            {
                M[i][j] = scores_repeat[i][j] + trans[i][j] + observe_repeat[i][j];
            }
            
        }
        // vector<float> scores_max;
        scores.clear();
        vector<int> idxs;
        for (size_t i = 0; i < M[0].size(); i++)
        {
            float max = 0.0f;
            int idx = 0;
            for (size_t j = 0; j < M.size(); j++)
            {
                if(M[j][i] > max) {
                    max = M[j][i];
                    idx = j;
                }
            }
            // scores_max.push_back(max);
            scores.push_back(max);
            idxs.push_back(idx);
        }
        paths.push_back(idxs);
    }
    vector <int> beat_path(seq_len, 0);
    int scores_max_index = 0;
    float scores_max = 0.0f;
    for (size_t i = 0; i < scores.size(); i++)
    {
        if (scores[i] > scores_max)
        {
            scores_max = scores[i];
            scores_max_index = i;
        }
    }
    beat_path[seq_len - 1] = scores_max_index;
    for (int i = seq_len - 2; i >= 0; i--)
    {
        int index = beat_path[i+1];
        beat_path[i] = paths[i][index];
    }
    return beat_path;
    
}
vector<string> split(const string& str, const string& pattern)
{
    vector<string> ret;
    if(pattern.empty()) return ret;
    size_t start=0,index=str.find_first_of(pattern,0);
    while(index!=str.npos)
    {
        if(start!=index)
            ret.push_back(str.substr(start,index-start));
        start=index+1;
        index=str.find_first_of(pattern,start);
    }
    if(!str.substr(start).empty())
        ret.push_back(str.substr(start));
    return ret;
}
int processviterbi() {

// map<int, string> ID_Name = {
//                 { 2015, "Jim" },
//                 { 2016, "Tom" },
//                 { 2017, "Bob" } };

    map<string, int> mymap;

    // 插入单个值
    mymap.insert(pair<string, int>("我", 100));
    mymap.insert(pair<string, int>("你", 200));
    mymap.insert(pair<string, int>("@", 200));

    // if(m.count(key)>0)
    // {
    //     return m[key];
    // }
    printf("get map value %d \n", mymap["@"]);
// # nodes.shape=[seq_len, num_labels],
// # trans.shape=[num_labels, num_labels].
// # 李三 爱 中国
// # labels D 0 N 1 R 2

// nodes = np.array([[0.2,0.1,0.7], [0.2,0.1,0.7], [0.7, 0.1,0.2], [0.1,0.7,0.2],[0.1,0.7,0.2]])

// #       D   N   R
// #  D   0.1  0.9 0.2
// #  N   0.3  0.5 0.1
// #  R   0.9  0.1 0.5

// trans = np.array([[0.1 , 0.9 ,0.2],
//         [0.3 , 0.5 ,0.1],
//         [0.9 , 0.1 ,0.5]])

// int b[5] = {1,2,3,4,5};
// a.push_back(b);
//float arrHeight[] = { 1.68,1.72,1.83,2.05,2.35,1.78,2.1,1.96 };  
//vector<float> vecHeight(arrHeight, arrHeight+sizeof(arrHeight)/sizeof(float));
// 二维数组：
// b[5][6] = {1,2,3,4,5,6...27,28,29,30};
// 如果a的长度给定了，即vector<vector<int> > a(5,vector<double>(6));


// for(int i = 0; i < 5；++i){
//     for (int j = 0; j < 6; ++j){
//         a[i][j]=b[i][j];
//     }
// }
    
    float nodes[5][3] = {{0.2,0.1,0.7},{0.2,0.1,0.7},{0.7, 0.1,0.2},{0.1,0.7,0.2},{0.1,0.7,0.2}};
    vector<vector<float> > nodesArrary(5,vector<float>(3));
    for (size_t i = 0; i < 5; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            nodesArrary[i][j]=nodes[i][j];
        }
        
    }
    float trans[3][3] = {{0.1 , 0.9 ,0.2}, {0.3 , 0.5 ,0.1}, {0.9 , 0.1 ,0.5}};
    vector<vector<float> > transArrary(3,vector<float>(3));

    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            transArrary[i][j]=trans[i][j];
        }
        
    }
    vector<int> path = viterbi_decode(nodesArrary, transArrary);

    for (size_t i = 0; i < path.size(); i++)
    {
        printf(" %d \t", path[i]);
    }
    printf(" \n");

    int nodes_row = 14;
    int nodes_col = 15;
    float nodes2[14][15] = {{-0.07018120,  0.03660753, -0.04121647,  0.22934404, -0.02582321,  0.05680982, -0.10411604, -0.04954027, -0.07821029, -0.00520372,  0.01084608,  0.23080766, -0.02660460, -0.09964336, -0.09398442},
         {-0.07566695,  0.04222254, -0.05179530,  0.29339629, -0.03809791,  0.04974633, -0.11507493, -0.05460043, -0.09101900, -0.01193390,  0.00967221,  0.27670619, -0.03882468, -0.12682508, -0.11578797},
         {-0.07847570,  0.04742332, -0.05921032,  0.34349895, -0.04763199,  0.04410050, -0.12520425, -0.05414520, -0.10092863, -0.01676221,  0.00696572,  0.31570971, -0.04690624, -0.14403026, -0.13136873},
         {-0.08000791,  0.05162270, -0.06403188,  0.37786546, -0.05434248,  0.04028274, -0.13220859, -0.05316375, -0.10735451, -0.02043603,  0.00423998,  0.34566736, -0.05200392, -0.15532401, -0.14098470},
         {-0.08099541,  0.05382394, -0.06627239,  0.40133718, -0.05860503,  0.03620201, -0.13745564, -0.05147804, -0.11152551, -0.02214665,  0.00203888,  0.36609638, -0.05483350, -0.16187121, -0.14881229},
         {-0.08179553,  0.05532463, -0.06831683,  0.41438270, -0.06128921,  0.03282732, -0.14011982, -0.04842233, -0.11282975, -0.02351534, -0.00004127,  0.37763509, -0.05659712, -0.16567051, -0.15396741},
         {-0.08146100,  0.05615698, -0.06818371,  0.42002028, -0.06251217,  0.03001436, -0.14083275, -0.04545075, -0.11258208, -0.02489891, -0.00129696,  0.38387099, -0.05723446, -0.16758987, -0.15567032},
         {-0.07955540,  0.05616137, -0.06721350,  0.41915646, -0.06189056,  0.02699124, -0.13942307, -0.04352519, -0.11070997, -0.02575264, -0.00152919,  0.38541460, -0.05697119, -0.16872829, -0.15508609},
         {-0.07808176,  0.05494870, -0.06494407,  0.41184205, -0.06059260,  0.02410184, -0.13592917, -0.04119680, -0.10714902, -0.02644142, -0.00062333,  0.38095784, -0.05532374, -0.16752586, -0.15224281},
         {-0.07286998,  0.05287436, -0.06153335,  0.39788109, -0.05800783,  0.01964913, -0.13118759, -0.03745609, -0.10155939, -0.02779303,  0.00090578,  0.37078020, -0.05335816, -0.16458929, -0.14759509},
         {-0.06557819,  0.04998210, -0.05606643,  0.37469685, -0.05447880,  0.01453183, -0.12361452, -0.03256487, -0.09300818, -0.02904942,  0.00297986,  0.35315052, -0.05039047, -0.15862310, -0.13916540},
         {-0.05681382,  0.04532495, -0.04803578,  0.33835447, -0.05063316,  0.00810711, -0.11227918, -0.02572053, -0.08017395, -0.03041211,  0.00538668,  0.32506430, -0.04632049, -0.14837314, -0.12622498},
         {-0.04626163,  0.03893683, -0.03788900,  0.29182613, -0.04638164, -0.00114088, -0.09763996, -0.01848520, -0.06327761, -0.03219523,  0.00690191,  0.28307906, -0.04158274, -0.13487133, -0.10983609},
         {-0.03321739,  0.03206696, -0.02751305,  0.23021226, -0.04112994, -0.01398895, -0.07702734, -0.00984159, -0.04195857, -0.02955639,  0.00577726,  0.22182792, -0.03733796, -0.11758191, -0.09004825}};

   vector<vector<float> > nodesArrary2(nodes_row, vector<float>(nodes_col));
    for (size_t i = 0; i < nodes_row; i++)
    {
        for (size_t j = 0; j < nodes_col; j++)
        {
            nodesArrary2[i][j]=nodes2[i][j];
        }
        
    }

    int trans_row_col = 15;
    float trans2[15][15] = {{-0.20152706,  0.35617781, -0.28335863,  0.15387022, -0.04330873, -0.18085745,  0.42665786, -0.15487880, -0.33945385, -0.20118675, -0.10621223,  0.33133739, -0.18248969,  0.40007746,  0.00033745},
        {-0.30265197, -0.09227812, -0.24230452, -0.43478689,  0.01128492,  0.22682184,  0.19619536, -0.07577717, -0.14401290, -0.19618833,  0.21475184,  0.04437804,  0.27976888,  0.08342677,  0.13134634},
        { 0.16412598,  0.16005665, -0.40247774,  0.19309831,  0.42493373, -0.38081974,  0.01118997, -0.15713054,  0.05110526,  0.07111067, -0.23154242,  0.41420335,  0.10648054, -0.28393477,  0.16584754},
        { 0.30289906, -0.11028686,  0.27789080, -0.09582472, -0.27311540,  0.39330685,  0.17711169,  0.26339030,  0.17979133, -0.37760279,  0.17450833,  0.29776418,  0.13907218,  0.08160287, -0.09656870},
        {-0.44326296,  0.15033317,  0.22210753, -0.03633824, -0.29325891,  0.17371851,  0.22942388, -0.31108311, -0.01888981,  0.41870147,  0.24147671,  0.08333671,  0.32731712, -0.06576622,  0.28185004},
        {-0.16290122, -0.19850416,  0.37202609, -0.17579916,  0.16976196, -0.27745003, -0.42641476,  0.44116098, -0.14522320, -0.19237351,  0.07437783, -0.20491850,  0.42241788, -0.36182082, -0.02846217},
        { 0.05247128,  0.23125207, -0.20735796,  0.13913655,  0.27070040, -0.23992184, -0.23005596, -0.01370978,  0.17161572, -0.11173853, -0.41131464,  0.35574120, -0.09547898,  0.33380073,  0.36055577},
        { 0.35773140, -0.36233398,  0.23854625,  0.30568987,  0.20140868,  0.43719864,  0.17813700,  0.16760570, -0.12874293, -0.24865495,  0.33992386,  0.22687399,  0.31721395, -0.37185624,  0.11379951},
        {-0.09541622, -0.09831741, -0.40238076, -0.00785804,  0.33342183,  0.00518838,  0.08113205, -0.27597043,  0.01411536, -0.36959440,  0.22973645, -0.27761316,  0.04066822, -0.25741202, -0.08549318},
        {-0.23232776,  0.41014135,  0.38155204, -0.12801594, -0.12376943, -0.40114477,  0.20825344,  0.08196861, -0.34259909,  0.28315115, -0.28820491, -0.01504558,  0.10885745,  0.07119513,  0.36830914},
        {-0.06166375, -0.42144060,  0.11057919, -0.22719145,  0.35523826,  0.43081111, -0.09855479,  0.31127661, -0.18275040, -0.08275780, -0.37360093,  0.04371366,  0.16213179,  0.09824872, -0.12677011},
        { 0.02674344,  0.11932075,  0.21089840,  0.15476954,  0.02649435, -0.29103133, -0.05315325,  0.02203205, -0.25264663, -0.32828799,  0.05822766,  0.17934442, -0.44120225, -0.03813326, -0.14720240},
        {-0.03122506, -0.21601056,  0.39150739,  0.13461530, -0.37401992, -0.13148192,  0.32946360, -0.44360220, -0.28873250,  0.22329909, -0.05260047,  0.36244977,  0.04562724,  0.41033715, -0.15741578},
        {-0.42412013, -0.27623415,  0.34608567,  0.14241076,  0.04812923, -0.35844371,  0.17370784, -0.06690273,  0.01742482,  0.31683969, -0.14436382,  0.09776813,  0.16113412, -0.34964144,  0.31782520},
        {-0.43153554, -0.37671757, -0.39549905,  0.21865922, -0.21946022, -0.24314983,  0.07858491,  0.38819009, -0.07887128,  0.18788451, -0.00817361,  0.29523951, -0.40478078, -0.01185718,  0.13200527}};
    vector<vector<float> > transArrary2(trans_row_col, vector<float>(trans_row_col));

    for (size_t i = 0; i < trans_row_col; i++)
    {
        for (size_t j = 0; j < trans_row_col; j++)
        {
            transArrary2[i][j]=trans2[i][j];
        }
        
    }
    // vector<int> path2 = viterbi_decode(nodesArrary2, transArrary2);
    vector<int> path2;
    path2.push_back(4);
    path2.push_back(5);
    path2.push_back(5);
    path2.push_back(5);
    path2.push_back(6);
    path2.push_back(7);
    path2.push_back(7);
    path2.push_back(7);
    for (size_t i = 0; i < path2.size(); i++)
    {
        printf(" %d \t", path2[i]);
    }
    printf(" \n");
    printf("hello \n");
    map<int ,string>  my_tag_list;

    string tags[13] = {"P-B","P-I","T-B","T-I","A1-B","A1-I","A2-B","A2-I","A3-B","A3-I","A4-B","A4-I","O"};
    for (int i = 0; i < 13; ++i) {
        my_tag_list.insert(make_pair( i, tags[i]));
    }

    string str="黑 龙 江 省 双 鸭 山 市 尖 山 区 八 马 路 与 东 平 行 路 交 叉 口 北 4 0 米 韦 业 涛 1 8 6 0 0 0 0 9 1 7 2";
    vector<string> str_vec = split(str, " ");

    string words ="";
    vector<string> sent_out;
    vector<string> tags_out;
    for (size_t i = 0; i < path2.size(); i++)
    {
        printf("SeqLabeling out path :   %ld \t" , i);
        string tag = my_tag_list[path2[i]];
        int findindex = tag.find("-B");
        int compareret = tag.compare("O");
        bool tagfine = findindex > 0;
        bool tagcompare = tag.compare("O") == 0;
        if (tagfine || tagcompare) {
            if (words.size()) {
                sent_out.push_back(words);
            }
            vector<string> split_tag = split(tag, "-");
            tags_out.push_back(""+split_tag[0]);
            words ="";
            words.append("" + str_vec[i]);
        } else {
            words.append("" + str_vec[i]);
        }
    }
    if (sent_out.size() < tags_out.size()) {
        sent_out.push_back(words);
    }
    for (int i = 0; i < sent_out.size(); ++i) {
        printf("sentences %s \t", sent_out[i].c_str());
        printf("tag %s \t", tags_out[i].c_str());

    }
}



