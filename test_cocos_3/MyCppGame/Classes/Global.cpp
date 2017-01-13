#include "Global.h"
#include "HallScene.h"
#include "tcpcommand.h"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#define key_BackgroundMusic "background_music_game"
#define key_Effect          "effect_game"

#define EFFECT_sendcard     "music/sendcard.wav"
#define EFFECT_place        "music/place.wav"
#define EFFECT_timer        "music/timer.wav"
#define EFFECT_warning      "music/warning.wav"
#define EFFECT_add_gold     "music/add_gold.wav"
#define EFFECT_select       "music/select.wav"
#define EFFECT_button       "music/button.wav"

//音效文件
//Android平台只支持OGG的音效格式
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define EFFECT_FILE        "music/effect2.ogg"
#elif( CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
#define EFFECT_FILE        "music/effect1.raw"
#else
#define EFFECT_FILE        "music/effect1.wav"
#endif

//音乐文件
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define MUSIC_FILE        "music/music.mid"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX )
#define MUSIC_FILE        "music/background.ogg"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
#define MUSIC_FILE        "music/background.wav"
#else
#define MUSIC_FILE        "music/background.mp3"
#endif


PostRef::PostRef(){
    
}

PostRef::~PostRef(){
    
}

static Global* share = nullptr;

Global::~Global(void){
    user_data = {0};
}
Global* Global::getInstance(){
    if(!share){
        share = new (std::nothrow) Global();
        
        Director::getInstance()->getScheduler()->scheduleSelector(
                                                                  schedule_selector(MTNotificationQueue::postNotifications),
                                                                  MTNotificationQueue::sharedNotificationQueue(),
                                                                  1.0 / 60.0,
                                                                  false);
        
        //预加载音乐音效
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(MUSIC_FILE);
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(EFFECT_FILE);
        
        int nNum = 0x12345678;
        char chData = *(char*)(&nNum);
        share->endianBig = (chData == 0x12);
    }
    
    return share;
}

bool Global::isBackgroundMusic(){
    bool canPlay = UserDefault::getInstance()->getBoolForKey(key_BackgroundMusic);
    
    return canPlay;
}

void Global::setBackgroundMusic(bool yn){
    UserDefault::getInstance()->setBoolForKey(key_BackgroundMusic, yn);
    playBackgroundMusic(yn);
}

void Global::playBackgroundMusic(bool yn){
    if (yn) {
        //        if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
        //
        //        }
        bool canPlay = isBackgroundMusic();
        if (canPlay) {
            SimpleAudioEngine::getInstance()->playBackgroundMusic(MUSIC_FILE, true);//播放音乐，循环
        }
    }
    else {
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }
}

bool Global::isEffect(){
    return UserDefault::getInstance()->getBoolForKey(key_Effect);
}
void Global::setEffect(bool yn){
    UserDefault::getInstance()->setBoolForKey(key_Effect, yn);
    if (!yn) {
        SimpleAudioEngine::getInstance()->stopAllEffects();
    }
}

void Global::stopEffect(int ID){
    SimpleAudioEngine::getInstance()->stopEffect(ID);
}

int Global::playEffect(const char* fileName , bool loop){
    if (isEffect()) {
        return SimpleAudioEngine::getInstance()->playEffect(fileName, loop);
    }
    
    return -1;
}
//发牌声
int Global::playEffect_sendcard(bool loop){
    return playEffect(EFFECT_sendcard, loop);
}

//请下注
int Global::playEffect_place(bool loop){
    return playEffect(EFFECT_place, loop);
}

//计时
int Global::playEffect_timer(bool loop){
    return playEffect(EFFECT_timer, loop);
}

//警告
int Global::playEffect_warning(bool loop){
    return playEffect(EFFECT_warning, loop);
}

int Global::playEffect_add_gold(bool loop){
    return playEffect(EFFECT_add_gold, loop);
}
int Global::playEffect_select(bool loop){
    return playEffect(EFFECT_select, loop);
}

int Global::playEffect_button(bool loop){
    return playEffect(EFFECT_button, loop);
}


void Global::saveLoginData(const rapidjson::Value& val_content){
    user_data = {0};
    parseUserData(val_content, &user_data);
    
    auto scene = Hall::createScene();
    Director::getInstance()->replaceScene(scene);
    
    this->connectServer();
}

void Global::logout(){
    Director::getInstance()->popToRootScene();
    this->playBackgroundMusic(false);
    
    this->disconnectServer();
}

int Global::calculateVIPLevel(int introCount){
    int level = 0;
    
    int levelArray[8] = {4,10,15,20,25,30,35,40};
    for (int i = 8; i > 0; i--) {
        if (introCount >= levelArray[i - 1]) {
            level = i;
            break;
        }
    }
    
    return level;
}

int Global::getInt(char *buffer, int offset) {
    return buffer[offset + 0] << 24 | (buffer[offset + 1] & 0xff) << 16 | (buffer[offset + 2] & 0xff) << 8 | (buffer[offset + 3] & 0xff);
}

void Global::parseUserData(const rapidjson::Value& val_user, UserData* data_user){
    data_user->gameTimes = val_user["gameTimes"].GetInt();
    
    const char* nikename = val_user["nikename"].GetString();
    memcpy(data_user->nikename, nikename, strlen(nikename));
    
    const char* account = val_user["account"].GetString();
    memcpy(data_user->account, account, strlen(account));
    
    const char* ID = val_user["id"].GetString();
    memcpy(data_user->ID, ID, strlen(ID));
    
    const char* winningPercent = val_user["winningPercent"].GetString();
    memcpy(data_user->winningPercent, winningPercent, strlen(winningPercent));
    
    const char* inviteCode = val_user["inviteCode"].GetString();
    memcpy(data_user->inviteCode, inviteCode, strlen(inviteCode));
    
    if (val_user.HasMember("introCount")) {
        data_user->introCount = val_user["introCount"].GetInt();
    }
    
    if (val_user.HasMember("diamondGameBit")) {
        const rapidjson::Value& val_diamondGameBit = val_user["diamondGameBit"];
        data_user->diamond = val_diamondGameBit["amount"].GetInt();
    }
    
    if (val_user.HasMember("silverGameBit")) {
        const rapidjson::Value& val_silverGameBit = val_user["silverGameBit"];
        data_user->silver = val_silverGameBit["amount"].GetInt();
    }
    
    if (val_user.HasMember("goldGameBit")) {
        const rapidjson::Value& val_goldGameBit = val_user["goldGameBit"];
        data_user->gold = val_goldGameBit["amount"].GetInt();
    }
}

void Global::parsePlayerData(const rapidjson::Value& val_player, PlayerData* data_player){
    data_player->capital = val_player["capital"].GetInt();
    data_player->remainCap = val_player["remainCap"].GetInt();
    if (val_player.HasMember("user")) {
        parseUserData(val_player["user"], &data_player->user);
        if (0 == strcmp(data_player->user.account, user_data.account)) {
            user_data.gold = data_player->user.gold;
            user_data.diamond = data_player->user.diamond;
            user_data.silver = data_player->user.silver;
        }
    }
}

void Global::clearPlayerList(){
    memset(playerList, 0, sizeof(PlayerData) * MAX_PLAYER_NUM);
    playerListCount = 0;
}

void Global::clearPokerSendedList(){
    memset(pokerSendedList, 0, sizeof(PokerPair) * 4);
    
}

#pragma Socket
void Global::disconnectServer(){
    // 关闭连接
    socket.Close();
    socket.Clean();
}
void Global::connectServer(){
    // 初始化
    // ODSocket socket;
    socket.Init();
    socket.Create(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // 设置服务器的IP地址，端口号
    // 并连接服务器 Connect
    const char* ip = "115.28.109.174";
    int port = 8888;
    bool result = socket.Connect(ip, port);
    
    if (result) {
        this->socketdidConnect();
    }
    else {
        log("Socket::can not connect to server");
        return;
    }
}

// 接收数据
void Global::receiveData(){
    // 因为是强联网
    // 所以可以一直检测服务端是否有数据传来
    while (true) {
        // 接收数据 Recv
        char data[MAX_NET_DATA_LEN] = "";
        int result = socket.Recv(data, MAX_NET_DATA_LEN, 0);
        // 与服务器的连接断开了
        if (result <= 0){
            socketDidDisconnect();
            break;
        }
        
        onReceiveData(data, result);
    }
    
}

void Global::sendData(const char* value){
    //    rapidjson::Document d1;
    //    d1.SetObject();
    //    rapidjson::Document::AllocatorType& allocator = d1.GetAllocator();
    //    rapidjson::Value array(rapidjson::kArrayType);
    //    rapidjson::Value object(rapidjson::kObjectType);
    //
    //    object.AddMember("id", 1000, allocator);
    //    object.AddMember("name", "234", allocator);
    //    object.AddMember("年龄", "111", allocator);
    //    //            array.PushBack(object, allocator);
    //
    //    d1.AddMember("propety", "PLAYER-TO", allocator);
    //    d1.AddMember("id", 3000, allocator);
    //    d1.AddMember("player", object, allocator);
    //
    //    rapidjson::StringBuffer buffer;
    //    rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
    //    d1.Accept(write);
    //
    //    //            StringUtils::format("%s",buffer.GetString());
    //    log("******%s",buffer.GetString());
    
    
    
    char send_data[1024] = {0};
    int value_len = (int)strlen(value);
    int reverseLen = reversebytes_uint32t(value_len);
    memcpy(send_data, &reverseLen, 4);
    memcpy(send_data + 4, value, value_len);
    
    int result_send = socket.Send(send_data, 4 + value_len);
    if (result_send > 0) {
        log("Socket::send->%s",value);
    }
}

void Global::socketdidConnect(){
    log("Socket::connect to server success!");
    
    // 开启新线程，在子线程中，接收数据
    std::thread recvThread = std::thread(&Global::receiveData, this);
    recvThread.detach(); // 从主线程分离
    
    sendHandle();
    //    //发送数据 Send
    //    SEND_PACKAGE package = {0};
    //    char handle[200];
    //    sprintf(handle, "{\"id\":1000,\"content\":{\"userId\":\"%s\"}}", user_data.ID);
    //
    //    int length = (int)strlen(handle);
    //    if (!endianBig) {
    //        package.valueLength = reversebytes_uint32t(length);
    //    }
    //    else {
    //        package.valueLength = length;
    //    }
    //
    //    memcpy(package.value, handle, length);
    //
    //    int result_send = socket.Send((const char *)&package, sizeof(int) + length);
    //    if (result_send > 0) {
    //        log("Socket::send->%s",package.value);
    //    }
}
void Global::socketDidDisconnect(){
    log("Socket::disconnect");
    
//    postNotification(cmd_disconnect);
}

void Global::onReceiveData(char *buffer, int len){
    log("Socket::receive->length:%d", len);
    
    bool parseHeader = false;
    for(int i = 0; i < len; i++){
        m_ucRecvBuffer[m_nRecvLen++] = buffer[i];
        if(m_nRecvLen == 4){
            if (!parseHeader) {
                int header = getInt(m_ucRecvBuffer, 0);
                m_nRecvFrameLen = header;
                m_nRecvLen = 0;
                parseHeader = true;
                continue;
            }
        }
        else if(m_nRecvLen == m_nRecvFrameLen){
            parseData(m_ucRecvBuffer, m_nRecvLen);
            
            memset(m_ucRecvBuffer, 0, m_nRecvLen);
            parseHeader = false;
            m_nRecvLen = 0;
            m_nRecvFrameLen = 0;
        }
        
    }
}

void Global::parseData(char* pbuf, int len){
    log("Socket::parse->%s", pbuf);
    
    rapidjson::Document document;
    document.Parse<0>(pbuf);
    CCASSERT(!document.HasParseError(), "Parsing to document failed");
    
    if(document.IsObject()){
        if(document.HasMember("code") && document.HasMember("id") && document.HasMember("content")){
            int code = document["code"].GetInt();
            int cmd = document["id"].GetInt();
            
            PostRef* post = new PostRef();
            post->cmd = cmd;
            memset(post->description, 0, sizeof(post->description));
            
            if (code == 1) {
                switch (cmd) {
                    case cmd_handle:{
                        //握手
                    }
                        break;
                        
                        //加入房间
                    case cmd_enterRoom:
                        //加入密码房间
                    case cmd_enterRoomByPassword:{
                        memset(&table_data, 0, sizeof(TableData));
                        
                        rapidjson::Value& val_content = document["content"];
                        
                        table_data.code = val_content["code"].GetInt();
                        
                        if (val_content.HasMember("tableId")) {
                            const char* tableId = val_content["tableId"].GetString();
                            memcpy(table_data.tableId, tableId, strlen(tableId));
                        }
                        
                        if (val_content.HasMember("room")) {
                            rapidjson::Value& val_room = val_content["room"];
                            
                            if (val_room.IsObject()) {
                                table_data.minStack = atoi(val_room["minStack"].GetString());
                                table_data.minPerStack= atoi(val_room["minPerStack"].GetString());
                                const char* roomId = val_room["id"].GetString();
                                memcpy(table_data.roomId, roomId, strlen(roomId));
                                
                                const char* roomType = val_room["roomType"].GetString();
                                memcpy(table_data.roomType, roomType, strlen(roomType));
                            }
                        }
                        
                        const char* description = val_content["description"].GetString();
                        memcpy(table_data.description, description, strlen(description));
                    }
                        break;
                        
                    case cmd_leaveRoom:{
                        //退出房间
                        rapidjson::Value& val_content = document["content"];
                        
                        memset(&table_data, 0, sizeof(TableData));
                        
                        table_data.code = val_content["code"].GetInt();
                        
                        const char* description = val_content["description"].GetString();
                        memcpy(table_data.description, description, strlen(description));
                    }
                        break;
                        
                    case cmd_playerReady:{
                        
                    }
                        break;
                        
                    case cmd_applyOwner:{
                        //抢庄
                        rapidjson::Value& val_content = document["content"];
                        
                        const char* description = val_content["description"].GetString();
                        memcpy(post->description, description, strlen(description));
                    }
                        break;
                        
                    case cmd_applyStabber:{
                        //抢刺
                        rapidjson::Value& val_content = document["content"];
                        
                        const char* description = val_content["description"].GetString();
                        memcpy(post->description, description, strlen(description));
                    }
                        break;
                        
                    case cmd_betStake:{
                        //下注
                        rapidjson::Value& val_content = document["content"];
                        
                        const char* description = val_content["description"].GetString();
                        memcpy(post->description, description, strlen(description));
                    }
                        break;
                        
                    default:
                        break;
                }
            }
            else {
                const char* description = document["content"].GetString();
                memcpy(post->description, description, strlen(description));
            }
            
            postNotification(post);
        }
        else if(document.HasMember("commandId") && document.HasMember("content")){
            int commandId = document["commandId"].GetInt();
            switch (commandId) {
                case cmd_removePlayer:{
                    //踢出房间
                    
                    
                }
                    break;
                    
                case cmd_beginCountDownBeforeBureau:{
                    //牌局开始前倒计时
                    countDownInSecond = document["content"].GetInt();
                    const char* tableId = document["tableId"].GetString();
                    if (0 != strcmp(tableId, table_data.tableId)) {
                        
                        return;
                    }
                    
                }
                    break;
                    
                case cmd_synPlayerList:{
                    //同步玩家列表
                    rapidjson::Value& val_content = document["content"];
                    
                    const char* tableId = document["tableId"].GetString();
                    if (0 != strcmp(tableId, table_data.tableId)) {
                        
                        return;
                    }
                    
                    if (val_content.IsArray()) {
                        clearPlayerList();
                        
                        playerListCount = val_content.Size();
                        for (int i = 0; i < playerListCount; ++i) {
                            rapidjson::Value& val_player = val_content[i];
                            assert(val_player.IsObject());
                            
                            PlayerData player_buf = {0};
                            parsePlayerData(val_player, &player_buf);
                            
                            playerList[i] = player_buf;
                        }
                    }
                }
                    break;
                    
                case cmd_bureauOpen:{
                    //开始牌局
                    rapidjson::Value& val_content = document["content"];
                    const char* tableId = document["tableId"].GetString();
                    if (0 != strcmp(tableId, table_data.tableId)) {
                        
                        return;
                    }
                    
                    const char* bureauId = val_content["bureauId"].GetString();
                    memcpy(table_data.bureauId, bureauId, strlen(bureauId));
                }
                    break;
                case cmd_countDownApplyBureauOwner:{
                    //开始抢庄倒计时
                    countDownInSecond = document["content"].GetInt();
                    const char* tableId = document["tableId"].GetString();
                    if (0 != strcmp(tableId, table_data.tableId)) {
                        
                        return;
                    }
                    
                    
                }
                    break;
                case cmd_selectedBureauOwner:{
                    //选中庄家通知
                    rapidjson::Value& val_content = document["content"];
                    const char* tableId = document["tableId"].GetString();
                    const char* bureauId = val_content["bureauId"].GetString();
                    if (0 != strcmp(tableId, table_data.tableId) || 0 != strcmp(bureauId, table_data.bureauId)) {
                        
                        return;
                    }
                    
                    const char* bureauOwnerId = val_content["bureauOwnerId"].GetString();
                    memcpy(table_data.bureauOwnerId, bureauOwnerId, strlen(bureauOwnerId));
                    
                    //判断当前玩家是否是庄家
                    isDealer = (0 == strcmp(table_data.bureauOwnerId, user_data.ID));
                }
                    break;
                    
                case cmd_countDownApplyStabber:{
                    //开始抢刺倒计时
                    rapidjson::Value& val_content = document["content"];
                    const char* tableId = document["tableId"].GetString();
                    if (0 != strcmp(tableId, table_data.tableId)) {
                        
                        return;
                    }
                    
                    countDownInSecond = val_content["countDown"].GetInt();
                    memset(table_data.roundId, 0, sizeof(table_data.roundId));
                    
                    const char* roundId = val_content["roundId"].GetString();
                    memcpy(table_data.roundId, roundId, strlen(roundId));
                }
                    break;
                    
                case cmd_notifyStabber:{
                    //抢中刺通知
                    rapidjson::Value& val_content = document["content"];
                    
                    const char* stabberId = val_content["userId"].GetString();
                    memcpy(table_data.stabberId, stabberId, strlen(stabberId));
                    
                    table_data.stabberIndex = val_content["type"].GetInt();
                }
                    break;
                    
                case cmd_countDownBetStake:{
                    //开始下注倒计时
                    rapidjson::Value& val_content = document["content"];
                    const char* tableId = document["tableId"].GetString();
                    if (0 != strcmp(tableId, table_data.tableId)) {
                        
                        return;
                    }
                    
                    countDownInSecond = val_content["countDown"].GetInt();
                    memset(table_data.roundId, 0, sizeof(table_data.roundId));
                    
                    const char* roundId = val_content["roundId"].GetString();
                    memcpy(table_data.roundId, roundId, strlen(roundId));
                }
                    break;
                    
                case cmd_countDownSendCard:{
                    //发牌
                    countDownInSecond = 10;
                    
                    rapidjson::Value& val_content = document["content"];
                    
                    const char* tableId = document["tableId"].GetString();
                    if (0 != strcmp(tableId, table_data.tableId)) {
                        
                        return;
                    }
                    
                    if (val_content.IsObject()) {
                        countDownInSecond = val_content["countDown"].GetInt();
                        
                        rapidjson::Value& val_gateCards = val_content["gateCards"];
                        rapidjson::Value& val_startCard = val_content["startCard"];
                        
                        if (val_gateCards.IsArray()) {
                            clearPokerSendedList();
                            
                            if (val_gateCards.Size() != 4) {
                                return;
                            }
                            for (int i = 0; i < val_gateCards.Size(); ++i) {
                                rapidjson::Value& val_pair = val_gateCards[i];
                                
                                PokerPair pair = {0};
                                pair.point = val_pair["point"].GetDouble();
                                
                                const char* pointDes = val_pair["pointDes"].GetString();
                                memcpy(pair.pointDes, pointDes, strlen(pointDes));
                                
                                rapidjson::Value& val_cards = val_pair["cards"];
                                if (val_cards.Size() == 2) {
                                    for (int j = 0; j < val_cards.Size(); ++j) {
                                        rapidjson::Value& card = val_cards[j];
                                        
                                        pair.poker[j].color = card["color"].GetInt();
                                        pair.poker[j].count = card["count"].GetDouble();
                                        pair.poker[j].num = card["num"].GetInt();
                                    }
                                }
                                
                                int gateType = val_pair["type"].GetInt();
                                
                                pokerSendedList[(gateType - 1) % 4] = pair;
                            }
                        }
                        if (val_startCard.IsObject()) {
                            memset(&pokerJudgement, 0, sizeof(PokerData));
                            
                            pokerJudgement.color = val_startCard["color"].GetInt();
                            pokerJudgement.count = val_startCard["count"].GetDouble();
                            pokerJudgement.num = val_startCard["num"].GetInt();
                        }
                    }
                    
                    
                }
                    break;
                    
                case cmd_sysBetStake:{
                    //同步下注数据
                    const char* tableId = document["tableId"].GetString();
                    if (0 != strcmp(tableId, table_data.tableId)) {
                        return;
                    }
                    
                    memset(betList, 0, sizeof(int) * 4);
                    rapidjson::Value& val_content = document["content"];
                    if (val_content.IsObject()) {
                        betList[1] = val_content["2"].GetInt();
                        betList[2] = val_content["3"].GetInt();
                        betList[3] = val_content["4"].GetInt();
                    }
                }
                    break;
                    
                case cmd_settle:{
                    //结算
                    const char* tableId = document["tableId"].GetString();
                    if (0 != strcmp(tableId, table_data.tableId)) {
                        return;
                    }
                    
                    memset(settleList, 0, sizeof(int) * 4);
                    
                    rapidjson::Value& val_content = document["content"];
                    
                    countDownInSecond = val_content["countDown"].GetInt();
                    
                    if (val_content.HasMember("accountResult")) {
                        rapidjson::Value& val_accountResult = val_content["accountResult"];
                        if (val_accountResult.IsArray()) {
                            for (int i = 0; i < val_accountResult.Size(); ++i) {
                                rapidjson::Value& val_settle = val_accountResult[i];
                                
                                int isOwner = val_settle["isOwner"].GetInt();
                                if (isOwner == 0) {
                                    if (isDealer) {
                                        rapidjson::Value& val_resultMap = val_settle["resultMap"];
                                        
                                        for (int j = 2; j <= 4; ++j) {
                                            char gate_settle[10] = {0};
                                            sprintf(gate_settle, "%d", j);
                                            if (val_resultMap.HasMember(gate_settle)) {
                                                rapidjson::Value& val_gate = val_resultMap[gate_settle];
                                                int count = val_gate["count"].GetInt();
                                                this->settleList[j - 1] -= count;
                                            }
                                        }
                                    }
                                    else {
                                        const char* userId = val_settle["userId"].GetString();
                                        if (0 == strcmp(userId, user_data.ID)) {
                                            rapidjson::Value& val_resultMap = val_settle["resultMap"];
                                            
                                            for (int j = 2; j <= 4; ++j) {
                                                char gate_settle[10] = {0};
                                                sprintf(gate_settle, "%d", j);
                                                if (val_resultMap.HasMember(gate_settle)) {
                                                    rapidjson::Value& val_gate = val_resultMap[gate_settle];
                                                    int count = val_gate["count"].GetInt();
                                                    this->settleList[j - 1] += count;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    
                    
                }
                    break;
                    
                default:
                    break;
            }
            postNotification(commandId);
        }
    }
}

void Global::postNotification(int cmd){
    PostRef* post = new PostRef();
    post->cmd = cmd;
    
    MTNotificationQueue::sharedNotificationQueue()->postNotification(kNotification_Socket, post);
}

void Global::postNotification(PostRef* post){
    MTNotificationQueue::sharedNotificationQueue()->postNotification(kNotification_Socket, post);
}

#pragma send cmd
void Global::sendHandle(){
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    
    doc.AddMember("id", cmd_handle, allocator);
    doc.AddMember("content", rapidjson::Value(user_data.ID, allocator), allocator);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
    doc.Accept(write);
    
    sendData(buffer.GetString());
}

void Global::sendEnterRoom(const char* roomTypeId, int capital){
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value content(rapidjson::kObjectType);
    
    content.AddMember("userId", rapidjson::Value(user_data.ID, allocator), allocator);
    content.AddMember("roomTypeId", rapidjson::Value(roomTypeId, allocator), allocator);
    content.AddMember("capital", capital, allocator);
    
    doc.AddMember("id", cmd_enterRoom, allocator);
    doc.AddMember("content", content, allocator);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
    doc.Accept(write);
    
    sendData(buffer.GetString());
}

void Global::sendEnterRoomByPassword(const char* roomPassword, int capital){
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value content(rapidjson::kObjectType);
    
    content.AddMember("userId", rapidjson::Value(user_data.ID, allocator), allocator);
    content.AddMember("roomPassword", rapidjson::Value(roomPassword, allocator), allocator);
    content.AddMember("capital", capital, allocator);
    
    doc.AddMember("id", cmd_enterRoomByPassword, allocator);
    doc.AddMember("content", content, allocator);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
    doc.Accept(write);
    
    sendData(buffer.GetString());
}

void Global::sendPlayerReady(){
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value content(rapidjson::kObjectType);
    
    content.AddMember("userId", rapidjson::Value(user_data.ID, allocator), allocator);
    content.AddMember("roomId", rapidjson::Value(table_data.roomId, allocator), allocator);
    content.AddMember("tableId", rapidjson::Value(table_data.tableId, allocator), allocator);
    
    doc.AddMember("id", cmd_playerReady, allocator);
    doc.AddMember("content", content, allocator);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
    doc.Accept(write);
    
    sendData(buffer.GetString());
}

void Global::sendApplyOwner(){
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value content(rapidjson::kObjectType);
    
    content.AddMember("userId", rapidjson::Value(user_data.ID, allocator), allocator);
    content.AddMember("roomId", rapidjson::Value(table_data.roomId, allocator), allocator);
    content.AddMember("bureauId", rapidjson::Value(table_data.bureauId, allocator), allocator);
    
    doc.AddMember("id", cmd_applyOwner, allocator);
    doc.AddMember("content", content, allocator);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
    doc.Accept(write);
    
    sendData(buffer.GetString());
}

void Global::sendApplyStabber(int gateType){
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value content(rapidjson::kObjectType);
    
    content.AddMember("userId", rapidjson::Value(user_data.ID, allocator), allocator);
    content.AddMember("roundId", rapidjson::Value(table_data.roundId, allocator), allocator);
    content.AddMember("type", gateType, allocator);
    
    doc.AddMember("id", cmd_applyStabber, allocator);
    doc.AddMember("content", content, allocator);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
    doc.Accept(write);
    
    sendData(buffer.GetString());
}

void Global::sendLeaveRoom(){
    if (strlen(table_data.roomId) == 0 || strlen(table_data.tableId) == 0 ) {
        postNotification(cmd_leaveRoom);
        return;
    }
    
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value content(rapidjson::kObjectType);
    
    content.AddMember("userId", rapidjson::Value(user_data.ID, allocator), allocator);
    content.AddMember("roomId", rapidjson::Value(table_data.roomId, allocator), allocator);
    content.AddMember("tableId", rapidjson::Value(table_data.tableId, allocator), allocator);
    
    doc.AddMember("id", cmd_leaveRoom, allocator);
    doc.AddMember("content", content, allocator);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
    doc.Accept(write);
    
    sendData(buffer.GetString());
}

void Global::sendBetStake(int jetton, int gateType){
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value content(rapidjson::kObjectType);
    
    content.AddMember("userId", rapidjson::Value(user_data.ID, allocator), allocator);
    content.AddMember("roundId", rapidjson::Value(table_data.roundId, allocator), allocator);
    content.AddMember("count", jetton, allocator);
    content.AddMember("gateType", gateType, allocator);
    
    doc.AddMember("id", cmd_betStake, allocator);
    doc.AddMember("content", content, allocator);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
    doc.Accept(write);
    
    sendData(buffer.GetString());
}
