#include "Global.h"
#include "HallScene.h"
#include "tcpcommand.h"
#include "OnlinePokerDeskScene.h"
#include "HelloWorldScene.h"

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

char colorString[7][20] = {"默认","亮黑色","黑色","银色","金色","玫瑰金","深空灰"};


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
//        Director::getInstance()->getScheduler()->scheduleUpdate(share, 1, false);
        
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
    
    if (this->isInRootScene) {
        Director::getInstance()->pushScene(scene);
    }
    else {
        Director::getInstance()->replaceScene(scene);
    }
    
    this->connectServer();
}

void Global::logout(){
    this->playBackgroundMusic(false);
    
    this->isInitiativeLogout = true;
    this->disconnectServer();
    
//    this->goToRootScene();
}

void Global::goToRootScene(){
    if (!this->isInRootScene) {
        Director::getInstance()->popToRootScene();
    }
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

bool Global::getStringWithItemColor(char *buffer, ItemColorType colorType){
    if (colorType >= ItemColorType_Default) {
        sprintf(buffer, "%s", colorString[colorType]);
        return true;
    }
    
    return false;
}

void Global::parseUserData(const rapidjson::Value& val_user, UserData* data_user){
    if (val_user.HasMember("gameTimes")) {
        data_user->gameTimes = val_user["gameTimes"].GetInt();
    }
    
    if (val_user.HasMember("nikename")) {
        const char* nikename = val_user["nikename"].GetString();
        memcpy(data_user->nikename, nikename, strlen(nikename));
    }
    
    if (val_user.HasMember("account")) {
        const char* account = val_user["account"].GetString();
        memcpy(data_user->account, account, strlen(account));
    }
    
    if (val_user.HasMember("id")) {
        const char* ID = val_user["id"].GetString();
        memcpy(data_user->ID, ID, strlen(ID));
    }
    
    if (val_user.HasMember("winningPercent")) {
        const char* winningPercent = val_user["winningPercent"].GetString();
        memcpy(data_user->winningPercent, winningPercent, strlen(winningPercent));
    }
    
    if (val_user.HasMember("inviteCode")) {
        const char* inviteCode = val_user["inviteCode"].GetString();
        memcpy(data_user->inviteCode, inviteCode, strlen(inviteCode));
    }
    
    if (val_user.HasMember("inviteUser")) {
        const rapidjson::Value& val_inviteUser = val_user["inviteUser"];
        if (!val_inviteUser.IsNull()) {
            const char* inviteUser = val_user["inviteUser"].GetString();
            memcpy(data_user->inviteUser, inviteUser, strlen(inviteUser));
        }
    }
    
    if (val_user.HasMember("introCount")) {
        data_user->introCount = val_user["introCount"].GetInt();
    }
    
    if (val_user.HasMember("diamondGameBit")) {
        const rapidjson::Value& val_diamondGameBit = val_user["diamondGameBit"];
        if (val_diamondGameBit.IsObject()) {
            if (val_diamondGameBit.HasMember("amount")) {
                data_user->diamond = val_diamondGameBit["amount"].GetInt();
            }
        }
    }
    
    if (val_user.HasMember("silverGameBit")) {
        const rapidjson::Value& val_silverGameBit = val_user["silverGameBit"];
        if (val_silverGameBit.IsObject()) {
            if (val_silverGameBit.HasMember("amount")) {
                data_user->silver = val_silverGameBit["amount"].GetInt();
            }
        }
    }
    
    if (val_user.HasMember("goldGameBit")) {
        const rapidjson::Value& val_goldGameBit = val_user["goldGameBit"];
        if (val_goldGameBit.IsObject()) {
            if (val_goldGameBit.HasMember("amount")) {
                data_user->gold = val_goldGameBit["amount"].GetInt();
            }
        }
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

void Global::parsePokerData(const rapidjson::Value& val_gateCards, const rapidjson::Value& val_startCard){
    if (val_gateCards.IsArray()) {
        clearPokerSendedList();
        
        if (val_gateCards.Size() != 4) {
            return;
        }
        for (int i = 0; i < val_gateCards.Size(); ++i) {
            const rapidjson::Value& val_pair = val_gateCards[i];
            
            PokerPair pair = {0};
            pair.point = val_pair["point"].GetDouble();
            
            const char* pointDes = val_pair["pointDes"].GetString();
            memcpy(pair.pointDes, pointDes, strlen(pointDes));
            
            const rapidjson::Value& val_cards = val_pair["cards"];
            if (val_cards.Size() == 2) {
                for (int j = 0; j < val_cards.Size(); ++j) {
                    const rapidjson::Value& card = val_cards[j];
                    
                    pair.poker[j].color = card["color"].GetInt();
                    pair.poker[j].count = card["count"].GetDouble();
                    pair.poker[j].num = card["num"].GetInt();
                }
            }
            
            int gateType = val_pair["type"].GetInt();
            
            table_data.round.pokerSendedList[(gateType - 1) % 4] = pair;
        }
    }
    if (val_startCard.IsObject()) {
        table_data.round.pokerJudgement.color = val_startCard["color"].GetInt();
        table_data.round.pokerJudgement.count = val_startCard["count"].GetDouble();
        table_data.round.pokerJudgement.num = val_startCard["num"].GetInt();
    }
}

void Global::parsePlayerStake(const rapidjson::Value& val_betStakes){
    if (val_betStakes.IsArray()) {
        for (int i = 0; i < val_betStakes.Size(); ++i) {
            const rapidjson::Value& val_stake = val_betStakes[i];
            if (val_stake.HasMember("count") && val_stake.HasMember("gate")) {
                int count = val_stake["count"].GetInt();
                int gate = atoi(val_stake["gate"].GetString());
                table_data.round.betStakes[(gate - 1) % 4] = count;
            }
        }
    }
}

void Global::clearPlayerList(){
    memset(playerList, 0, sizeof(PlayerData) * MAX_PLAYER_NUM);
    playerListCount = 0;
}


void Global::clearPokerSendedList(){
    memset(table_data.round.pokerSendedList, 0, sizeof(PokerPair) * 4);
}

//清除房间数据
void Global::clearRoomData(){
    
}

//清除桌子数据
void Global::clearTableData(){
    memset(&table_data, 0, sizeof(TableData));
}

//清除局数据
void Global::clearBureauData(){
    memset(&table_data.bureau, 0, sizeof(BureauData));
}

//清除局数据（庄家数据除外）
void Global::clearBureauDataWithoutOwner(){
    memset(table_data.bureau.bureauId, 0, sizeof(table_data.bureau.bureauId));
}

//清除把数据
void Global::clearRoundData(){
    memset(&table_data.round, 0, sizeof(RoundData));
}

//清除下注数据
void Global::clearBetData(){
    memset(table_data.round.betList, 0, sizeof(table_data.round.betList));
}

//清除庄家数据
void Global::clearBureauOwnerData(){
    memset(table_data.bureau.bureauOwnerId, 0, sizeof(table_data.bureau.bureauOwnerId));
}

//重置庄家数据
void Global::resetBureauOwnerData(const char* ownerId){
    clearBureauOwnerData();
    if (ownerId != NULL) {
        memcpy(table_data.bureau.bureauOwnerId, ownerId, strlen(ownerId));
    }
    //判断当前玩家是否是庄家
    isDealer = (0 == strcmp(table_data.bureau.bureauOwnerId, user_data.ID));
}

//把数重置
void Global::resetRoundIndex(){
    table_data.round.roundIndex = 1;
}

void Global::update(float delta){
//    // 接收消息处理（放到游戏主循环中，每帧处理）
//    if (!m_socket) {
//        return;
//    }
//    
//    if (!m_socket->Check()) {
//        m_socket = NULL;
//        // 掉线了
//        socketDidDisconnect();
//        return;
//    }
//    
//    // 发送数据（向服务器发送消息）
//    m_socket->Flush();
//    
//    // 接收数据（取得缓冲区中的所有消息，直到缓冲区为空）
//    while (true)
//    {
//        char buffer[_MAX_MSGSIZE] = { 0 };
//        int nSize = sizeof(buffer);
//        char* pbufMsg = buffer;
//        if(m_socket == NULL)
//        {
//            break;
//        }
//        if (!m_socket->ReceiveMsg(pbufMsg, nSize)) {
//            break;
//        }
//        
//        onReceiveData(pbufMsg, nSize);
//        break;
//    }
}

#pragma Socket
void* Global::threadFunc(void *arg){
    Global::getInstance()->receiveData();
    return NULL;
}

void Global::disconnectServer(){
    // 关闭连接
    m_socket.Close();
    m_socket.Clean();
}
void Global::connectServer(){
    // 初始化
    // ODSocket socket;
    m_socket.Init();
    m_socket.Create(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // 设置服务器的IP地址，端口号
    // 并连接服务器 Connect
    const char* ip = "115.28.109.174";
    int port = 8888;
    bool result = m_socket.Connect(ip, port);
    
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
        int result = m_socket.Recv(data, MAX_NET_DATA_LEN, 0);
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
    
    int result_send = m_socket.Send(send_data, 4 + value_len);
    if (result_send > 0) {
        log("Socket::send->%s",value);
    }
}

void Global::socketdidConnect(){
    log("Socket::connect to server success!");
    
//    // 开启新线程，在子线程中，接收数据
    std::thread recvThread = std::thread(&Global::receiveData, this);
    recvThread.detach(); // 从主线程分离
//    int errCode = 0;
//    do{
//        pthread_attr_t attributes;
//        errCode = pthread_attr_init(&attributes);
//        CC_BREAK_IF(errCode != 0);
//        //但是上面这个函数其他内容则主要为你创建的线程设定为分离式
//        errCode = pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED);
//        if (errCode != 0) {
//            pthread_attr_destroy(&attributes);
//            break;
//        }
//        errCode = pthread_create(&handle, &attributes,threadFunc,this);
//    }while (0);
    
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
    //    int result_send = m_socket.Send((const char *)&package, sizeof(int) + length);
    //    if (result_send > 0) {
    //        log("Socket::send->%s",package.value);
    //    }
}

void Global::socketDidDisconnect(){
    log("Socket::disconnect");
    
    this->goToRootScene();
    postNotification(cmd_disconnect);
}

void Global::onReceiveData(char *buffer, int len){
    log("Socket::receive->length:%d", len);
    
    for(int i = 0; i < len; i++){
        m_ucRecvBuffer[m_nRecvLen++] = buffer[i];
        if(m_nRecvLen <= 4){
            if (m_nRecvLen == 4) {
                int header = getInt(m_ucRecvBuffer, 0);
                m_nRecvFrameLen = header;
                if (m_nRecvFrameLen <= 0) {
                    memset(m_ucRecvBuffer, 0, m_nRecvLen);
                    m_nRecvLen = 0;
                    m_nRecvFrameLen = 0;
                }
            }
        }
        else if(m_nRecvLen - 4 == m_nRecvFrameLen){
            parseData(m_ucRecvBuffer + 4, m_nRecvFrameLen);
            
            memset(m_ucRecvBuffer, 0, m_nRecvLen);
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
            post->sub_cmd = code;
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
                        this->clearTableData();
                        
                        rapidjson::Value& val_content = document["content"];
                        
                        table_data.code = val_content["code"].GetInt();
                        
                        if (val_content.HasMember("tableId")) {
                            const char* tableId = val_content["tableId"].GetString();
                            memcpy(table_data.tableId, tableId, strlen(tableId));
                        }
                        
                        if (val_content.HasMember("countDown")) {
                            countDownInSecond = val_content["countDown"].GetInt();
                        }
                        
                        if (val_content.HasMember("room")) {
                            rapidjson::Value& val_room = val_content["room"];
                            
                            if (val_room.IsObject()) {
                                table_data.minStack = val_room["minStack"].GetInt();
                                table_data.minPerStack= val_room["minPerStack"].GetInt();
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
                        
                    case cmd_supplyBit:{
                        //补充本金
                        rapidjson::Value& val_content = document["content"];
                        
                        post->sub_cmd = val_content["code"].GetInt();
                        if (val_content.HasMember("description")) {
                            const char* description = val_content["description"].GetString();
                            memcpy(post->description, description, strlen(description));
                        }
                        
                        if (val_content.HasMember("userId") && val_content.HasMember("remindCap")) {
                            const char* userId = val_content["userId"].GetString();
                            int remindCap = val_content["remindCap"].GetInt();
                            
                            for (int i = 0; i < playerListCount; i++) {
                                PlayerData* player_data = &playerList[i];
                                if (0 == strcmp(userId, player_data->user.ID)) {
                                    player_data->remainCap = remindCap;
                                }
                            }
                        }
                        
                    }
                        break;
                        
                        //给所有人发送消息
                    case cmd_sendMessageToAll:{
                        const char* description = document["content"].GetString();
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
            
            if (commandId >= 10000) {
                //恢复牌局
                rapidjson::Value& val_content = document["content"];
                if (val_content.HasMember("tableId")) {
                    const char* tableId = val_content["tableId"].GetString();
                    memcpy(table_data.tableId, tableId, strlen(tableId));
                }
            }
            
            switch (commandId) {
                case cmd_removePlayer:{
                    //踢出房间
                    
                    
                }
                    break;
                    
                case cmd_beginCountDownBeforeBureau:{
                    //牌局开始前倒计时
                    this->resetRoundIndex();
                    
                    countDownInSecond = document["content"].GetInt();
                    
                    const char* tableId = document["tableId"].GetString();
                    if (strlen(table_data.tableId) > 0) {
                        if (0 != strcmp(tableId, table_data.tableId)) {
                            
                            return;
                        }
                    }
                    else {
                        memcpy(table_data.tableId, tableId, strlen(tableId));
                    }
                    
                }
                    break;
                    
                case cmd_synPlayerList:{
                    //同步玩家列表
                    rapidjson::Value& val_content = document["content"];
                    
//                    const char* tableId = document["tableId"].GetString();
//                    if (0 != strcmp(tableId, table_data.tableId)) {
//                        return;
//                    }
                    
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
                    this->clearBureauDataWithoutOwner();
                    
                    rapidjson::Value& val_content = document["content"];
                    const char* tableId = document["tableId"].GetString();
                    if (0 != strcmp(tableId, table_data.tableId)) {
                        
                        return;
                    }
                    
                    const char* bureauId = val_content["bureauId"].GetString();
                    memcpy(table_data.bureau.bureauId, bureauId, strlen(bureauId));
                }
                    break;
                    
                    //开始抢庄倒计时
                case cmd_countDownApplyBureauOwner:
                    //开始抢庄恢复
                case cmd_applyBureauOwnerRecover:{
                    rapidjson::Value& val_content = document["content"];
                    
                    const char* tableId = document["tableId"].GetString();
                    if (0 != strcmp(tableId, table_data.tableId)) {
                        
                        return;
                    }
                    
                    if (val_content.IsObject()) {
                        if (val_content.HasMember("curCount")) {
                            countDownInSecond = val_content["curCount"].GetInt();
                        }
                        
                        if (val_content.HasMember("bureauId")) {
                            const char* bureauId = val_content["bureauId"].GetString();
                            memcpy(table_data.bureau.bureauId, bureauId, strlen(bureauId));
                        }
                    }
                    else {
                        countDownInSecond = document["content"].GetInt();
                    }
                }
                    break;
                case cmd_selectedBureauOwner:{
                    //选中庄家通知
                    rapidjson::Value& val_content = document["content"];
                    const char* tableId = document["tableId"].GetString();
                    
                    if (0 != strcmp(tableId, table_data.tableId)) {
                        return;
                    }
                    
                    if (val_content.HasMember("bureauId")) {
                        const char* bureauId = val_content["bureauId"].GetString();
                        if (0 != strcmp(bureauId, table_data.bureau.bureauId)) {
                            return;
                        }
                    }
                    
                    if (!val_content.HasMember("bureauOwnerId")) {
                        return;
                    }
                    
                    const char* bureauOwnerId = val_content["bureauOwnerId"].GetString();
                    resetBureauOwnerData(bureauOwnerId);
                }
                    break;
                    
                    //开始抢刺倒计时
                case cmd_countDownApplyStabber:
                    //恢复抢刺
                case cmd_applyStabberRecover:{
                    
                    this->clearRoundData();
                    
                    rapidjson::Value& val_content = document["content"];
                    const char* tableId = document["tableId"].GetString();
                    if (0 != strcmp(tableId, table_data.tableId)) {
                        
                        return;
                    }
                    
                    if (val_content.HasMember("countDown")) {
                        countDownInSecond = val_content["countDown"].GetInt();
                    }
                    
                    if (val_content.HasMember("curCount")) {
                        countDownInSecond = val_content["curCount"].GetInt();
                    }
                    
                    if (val_content.HasMember("roundId")) {
                        memset(table_data.round.roundId, 0, sizeof(table_data.round.roundId));
                        const char* roundId = val_content["roundId"].GetString();
                        memcpy(table_data.round.roundId, roundId, strlen(roundId));
                    }
                    
                    if (val_content.HasMember("roundIndex")) {
                        table_data.round.roundIndex = val_content["roundIndex"].GetInt();
                    }
                    
                    if (val_content.HasMember("bureauId")) {
                        const char* bureauId = val_content["bureauId"].GetString();
                        memcpy(table_data.bureau.bureauId, bureauId, strlen(bureauId));
                    }
                    
                    if (val_content.HasMember("ownerId")) {
                        const char* ownerId = val_content["ownerId"].GetString();
                        resetBureauOwnerData(ownerId);
                    }
                }
                    break;
                    
                case cmd_notifyStabber:{
                    //抢中刺通知
                    rapidjson::Value& val_content = document["content"];
                    
                    const char* stabberId = val_content["userId"].GetString();
                    memcpy(table_data.round.stabberId, stabberId, strlen(stabberId));
                    
                    table_data.round.stabberIndex = val_content["type"].GetInt();
                }
                    break;
                    
                    //开始下注倒计时
                case cmd_countDownBetStake:
                    //恢复到下注
                case cmd_betStakeRecover:{
                    this->clearRoundData();
                    
                    rapidjson::Value& val_content = document["content"];
                    const char* tableId = document["tableId"].GetString();
                    if (0 != strcmp(tableId, table_data.tableId)) {
                        
                        return;
                    }
                    
                    if (val_content.HasMember("countDown")) {
                        countDownInSecond = val_content["countDown"].GetInt();
                    }
                    
                    if (val_content.HasMember("curCount")) {
                        countDownInSecond = val_content["curCount"].GetInt();
                    }
                    
                    if (val_content.HasMember("roundId")) {
                        memset(table_data.round.roundId, 0, sizeof(table_data.round.roundId));
                        const char* roundId = val_content["roundId"].GetString();
                        memcpy(table_data.round.roundId, roundId, strlen(roundId));
                    }
                    
                    if (val_content.HasMember("roundIndex")) {
                        table_data.round.roundIndex = val_content["roundIndex"].GetInt();
                    }
                    
                    if (val_content.HasMember("bureauId")) {
                        const char* bureauId = val_content["bureauId"].GetString();
                        memcpy(table_data.bureau.bureauId, bureauId, strlen(bureauId));
                    }
                    
                    if (val_content.HasMember("ownerId")) {
                        const char* ownerId = val_content["ownerId"].GetString();
                        resetBureauOwnerData(ownerId);
                    }
                    
                    if (val_content.HasMember("betStakes")) {
                        rapidjson::Value& val_betStakes = val_content["betStakes"];
                        this->parsePlayerStake(val_betStakes);
                    }
                }
                    break;
                    
                    //发牌
                case cmd_countDownSendCard:
                    //恢复到发牌
                case cmd_sendCardRecover:{
                    rapidjson::Value& val_content = document["content"];
                    
                    const char* tableId = document["tableId"].GetString();
                    if (0 != strcmp(tableId, table_data.tableId)) {
                        
                        return;
                    }
                    
                    if (val_content.IsObject()) {
                        if (val_content.HasMember("roundIndex")) {
                            table_data.round.roundIndex = val_content["roundIndex"].GetInt();
                        }
                        
                        if (val_content.HasMember("bureauId")) {
                            const char* bureauId = val_content["bureauId"].GetString();
                            memcpy(table_data.bureau.bureauId, bureauId, strlen(bureauId));
                        }
                        
                        if (val_content.HasMember("ownerId")) {
                            const char* ownerId = val_content["ownerId"].GetString();
                            resetBureauOwnerData(ownerId);
                        }
                        
                        if (val_content.HasMember("countDown")) {
                            countDownInSecond = val_content["countDown"].GetInt();
                        }
                        
                        if (val_content.HasMember("curCount")) {
                            countDownInSecond = val_content["curCount"].GetInt();
                        }
                        
                        if (val_content.HasMember("gateCards") && val_content.HasMember("startCard")) {
                            rapidjson::Value& val_gateCards = val_content["gateCards"];
                            rapidjson::Value& val_startCard = val_content["startCard"];
                            parsePokerData(val_gateCards, val_startCard);
                        }
                        
                        if (val_content.HasMember("betStakes")) {
                            rapidjson::Value& val_betStakes = val_content["betStakes"];
                            this->parsePlayerStake(val_betStakes);
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
                    
                    this->clearBetData();
                    rapidjson::Value& val_content = document["content"];
                    if (val_content.IsObject()) {
                        table_data.round.betList[1] = val_content["2"].GetInt();
                        table_data.round.betList[2] = val_content["3"].GetInt();
                        table_data.round.betList[3] = val_content["4"].GetInt();
                    }
                }
                    break;
                    
                    //结算
                case cmd_settle:
                    //结算恢复
                case cmd_settleRecover:{
                    const char* tableId = document["tableId"].GetString();
                    if (0 != strcmp(tableId, table_data.tableId)) {
                        return;
                    }
                    
                    rapidjson::Value& val_content = document["content"];
                    assert(val_content.IsObject());
                    
                    if (val_content.HasMember("roundIndex")) {
                        table_data.round.roundIndex = val_content["roundIndex"].GetInt();
                    }
                    
                    if (val_content.HasMember("bureauId")) {
                        const char* bureauId = val_content["bureauId"].GetString();
                        memcpy(table_data.bureau.bureauId, bureauId, strlen(bureauId));
                    }
                    
                    if (val_content.HasMember("ownerId")) {
                        const char* ownerId = val_content["ownerId"].GetString();
                        resetBureauOwnerData(ownerId);
                    }
                    
                    if (val_content.HasMember("countDown")) {
                        countDownInSecond = val_content["countDown"].GetInt();
                    }
                    
                    if (val_content.HasMember("curCount")) {
                        countDownInSecond = val_content["curCount"].GetInt();
                    }
                    
                    if (val_content.HasMember("gates") && val_content.HasMember("startCard")) {
                        rapidjson::Value& val_gateCards = val_content["gates"];
                        rapidjson::Value& val_startCard = val_content["startCard"];
                        parsePokerData(val_gateCards, val_startCard);
                    }
                    
                    if (val_content.HasMember("betStakes")) {
                        rapidjson::Value& val_betStakes = val_content["betStakes"];
                        this->parsePlayerStake(val_betStakes);
                    }
                    
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
                                                table_data.round.settleList[j - 1].count -= count;
                                                
                                                int times = val_gate["times"].GetInt();
                                                table_data.round.settleList[j - 1].times = -1 * times;
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
                                                    table_data.round.settleList[j - 1].count += count;
                                                    
                                                    int times = val_gate["times"].GetInt();
                                                    table_data.round.settleList[j - 1].times = times;
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
                    
                case cmd_bureauOwnerOff:{
                    //庄家强制下庄
                    const char* tableId = document["tableId"].GetString();
                    if (0 != strcmp(tableId, table_data.tableId)) {
                        return;
                    }
                    
                    resetBureauOwnerData(NULL);
                    
                    rapidjson::Value& val_content = document["content"];
                    if (val_content.IsObject()) {
                        if (val_content.HasMember("ownerLossReason") && val_content.HasMember("ownerUserId") && val_content.HasMember("bureauId")) {
                            
                            
                        }
                    }
                }
                    break;
                    
                    //收到所有人消息
                case cmd_receiveAllMessage:{
                    if (!document["content"].IsNull()) {
                        rapidjson::Value& val_content = document["content"];
                        if (!val_content["fromUserId"].IsNull() && !val_content["fromUserNikeName"].IsNull() && !val_content["message"].IsNull() && !val_content["sendTime"].IsNull()) {
                            MessageRef* message = new MessageRef();
                            message->autorelease();
                            
                            strcpy(message->fromUserId, val_content["fromUserId"].GetString());
                            strcpy(message->fromUserNikeName, val_content["fromUserNikeName"].GetString());
                            strcpy(message->message, val_content["message"].GetString());
                            strcpy(message->sendTime, val_content["sendTime"].GetString());
                            
                            this->messageItems.pushBack(message);
                        }
                    }
                }
                    break;
                    
                    //恢复牌局通知
                case cmd_notifyRecoverBureau:{
                    this->clearTableData();
                    
                    rapidjson::Value& val_content = document["content"];
                    
                    if (val_content.HasMember("code")) {
                        table_data.code = val_content["code"].GetInt();
                    }
                    
                    if (val_content.HasMember("tableId")) {
                        const char* tableId = val_content["tableId"].GetString();
                        memcpy(table_data.tableId, tableId, strlen(tableId));
                    }
                    
                    if (val_content.HasMember("room")) {
                        rapidjson::Value& val_room = val_content["room"];
                        
                        if (val_room.IsObject()) {
                            table_data.minStack = val_room["minStack"].GetInt();
                            table_data.minPerStack= val_room["minPerStack"].GetInt();
                            const char* roomId = val_room["id"].GetString();
                            memcpy(table_data.roomId, roomId, strlen(roomId));
                            
                            const char* roomType = val_room["roomType"].GetString();
                            memcpy(table_data.roomType, roomType, strlen(roomType));
                            
                            const char* roomTypeId = val_room["roomTypeId"].GetString();
                            memcpy(table_data.roomTypeId, roomTypeId, strlen(roomTypeId));
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

void Global::sendEnterRoomByPassword(const char* roomPassword, int capital, int type){
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value content(rapidjson::kObjectType);
    
    content.AddMember("userId", rapidjson::Value(user_data.ID, allocator), allocator);
    content.AddMember("roomPassword", rapidjson::Value(roomPassword, allocator), allocator);
    content.AddMember("capital", capital, allocator);
    content.AddMember("type", type, allocator);
    
    doc.AddMember("id", cmd_enterRoomByPassword, allocator);
    doc.AddMember("content", content, allocator);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
    doc.Accept(write);
    
    sendData(buffer.GetString());
}

void Global::sendEnterRoomByID(const char* roomId, int capital, int type){
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value content(rapidjson::kObjectType);
    
    content.AddMember("userId", rapidjson::Value(user_data.ID, allocator), allocator);
    content.AddMember("roomId", rapidjson::Value(roomId, allocator), allocator);
    content.AddMember("capital", capital, allocator);
    content.AddMember("type", type, allocator);
    
    doc.AddMember("id", cmd_enterRoomByRoomId, allocator);
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
    content.AddMember("bureauId", rapidjson::Value(table_data.bureau.bureauId, allocator), allocator);
    
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
    content.AddMember("roundId", rapidjson::Value(table_data.round.roundId, allocator), allocator);
    content.AddMember("type", gateType, allocator);
    
    doc.AddMember("id", cmd_applyStabber, allocator);
    doc.AddMember("content", content, allocator);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
    doc.Accept(write);
    
    sendData(buffer.GetString());
}

void Global::sendLeaveRoom(){
    if (strlen(table_data.roomId) == 0) {
        postNotification(cmd_leaveRoom);
        return;
    }
    
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value content(rapidjson::kObjectType);
    
    content.AddMember("userId", rapidjson::Value(user_data.ID, allocator), allocator);
    
    if (strlen(table_data.roomId) > 0) {
        content.AddMember("roomId", rapidjson::Value(table_data.roomId, allocator), allocator);
    }
    
    if (strlen(table_data.tableId) > 0) {
        content.AddMember("tableId", rapidjson::Value(table_data.tableId, allocator), allocator);
    }
    
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
    content.AddMember("roomId", rapidjson::Value(table_data.roomId, allocator), allocator);
    content.AddMember("roundId", rapidjson::Value(table_data.round.roundId, allocator), allocator);
    content.AddMember("count", jetton, allocator);
    content.AddMember("gateType", gateType, allocator);
    
    doc.AddMember("id", cmd_betStake, allocator);
    doc.AddMember("content", content, allocator);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
    doc.Accept(write);
    
    sendData(buffer.GetString());
}

void Global::sendSupplyBit(int count){
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value content(rapidjson::kObjectType);
    
    content.AddMember("userId", rapidjson::Value(user_data.ID, allocator), allocator);
    content.AddMember("tableId", rapidjson::Value(table_data.tableId, allocator), allocator);
    content.AddMember("roomId", rapidjson::Value(table_data.roomId, allocator), allocator);
    content.AddMember("count", count, allocator);
    
    doc.AddMember("id", cmd_supplyBit, allocator);
    doc.AddMember("content", content, allocator);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
    doc.Accept(write);
    
    sendData(buffer.GetString());
}

void Global::sendMessageToAll(const char* message){
    if (strlen(message) > Max_Message_Length) {
        return;
    }
    
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value content(rapidjson::kObjectType);
    
    content.AddMember("userId", rapidjson::Value(user_data.ID, allocator), allocator);
    content.AddMember("message", rapidjson::Value(message, allocator), allocator);
    
    doc.AddMember("id", cmd_sendMessageToAll, allocator);
    doc.AddMember("content", content, allocator);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> write(buffer);
    doc.Accept(write);
    
    sendData(buffer.GetString());
}

