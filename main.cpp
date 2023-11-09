/**
 * @Author 小皓
 * @Date 2023/08/13 21:01 原创
 */

#include <jni.h>
#include <time.h>
#include <cstring>
#include "Hack/ModuleManager.h"
Entity thePlayer;
GameSetting gameSetting;
bool load(JNIEnv* env, jobject obj, jstring name);
void setAirJump(JNIEnv* env, jobject obj, bool enable);
void setAutoBlock(JNIEnv* env, jobject obj);
void setFly(JNIEnv* env, jobject obj, bool enable);
void setCanFly(JNIEnv* env, jobject obj, bool enable);
void setJetPack(JNIEnv* env, jobject obj, bool enable, float speed);
void setbHop(JNIEnv* env, jobject obj, bool enable, float speed);
void setKillAura(JNIEnv* env, jobject obj, bool enable);
void setHitBox(JNIEnv* env, jobject obj, bool enable, float width, float hight);
void setTpAura(JNIEnv* env, jobject obj, bool enable, float x, float y, float z);
void setSurround(JNIEnv* env, jobject obj, bool enable, float r, float h, float speed);
void setLockBack(JNIEnv* env, jobject obj, bool enable, float r, float h);
void setNoClip(JNIEnv* env, jobject obj, bool enable);
void setClimb(JNIEnv* env, jobject obj, bool enable);
void setGameMode(JNIEnv* env, jobject obj, bool enable);
bool PlayerIsMove(JNIEnv* env, jobject obj);
long getLocalPlayer(JNIEnv* env, jobject obj);
float getPlayerRotX(JNIEnv* env, jobject obj, long player);
float getPlayerRotY(JNIEnv* env, jobject obj, long player);
float getPlayerPosX(JNIEnv* env, jobject obj, long player);
float getPlayerPosY(JNIEnv* env, jobject obj, long player);
float getPlayerPosZ(JNIEnv* env, jobject obj, long player);
void hackTick();

static JNINativeMethod ModuleCalled[] = { 
    {"load", "(Ljava/lang/String;)Z", (void *)load},
    {"setAirJump", "(Z)V", (void *)setAirJump},
    {"setFly", "(Z)V", (void *)setFly},
    {"setCanFly", "(Z)V", (void *)setCanFly},
    {"setJetPack", "(ZF)V", (void *)setJetPack},
    {"setbHop", "(ZF)V", (void *)setbHop},
    {"setKillAura", "(Z)V", (void *)setKillAura},
    {"setHitBox", "(ZFF)V", (void *)setHitBox},
    {"setTpAura", "(ZFFF)V", (void *)setTpAura},
    {"setSurround", "(ZFFF)V", (void *)setSurround},
    {"setLockBack", "(ZFF)V", (void *)setLockBack},
    {"setNoClip", "(Z)V", (void *)setNoClip},
    {"setClimb", "(Z)V", (void *)setClimb},
    {"setGameMode", "(Z)V", (void *)setGameMode},
    {"setAutoBlock", "()V", (void *)setAutoBlock},
    {"PlayerIsMove", "()Z", (void *)PlayerIsMove},
    {"getLocalPlayer", "()J", (void *)getLocalPlayer},
    {"getPlayerRotX", "(J)F", (void *)getPlayerRotX},
    {"getPlayerRotY", "(J)F", (void *)getPlayerRotY},
    {"getPlayerPosX", "(J)F", (void *)getPlayerPosX},
    {"getPlayerPosY", "(J)F", (void *)getPlayerPosY},
    {"getPlayerPosZ", "(J)F", (void *)getPlayerPosZ},
};

bool load(JNIEnv* env, jobject obj, jstring name){
    pid_ = getPID(jstringToChar(env, name));
    handle_ = getHandle(pid_);
    if(pid_ > 0 && handle_ > 0){
        std::thread hackThread(hackTick);
        hackThread.detach();
        return true;
    }
    return false;
}
void setAirJump(JNIEnv* env, jobject obj, bool enable){
    AirJump_enable = enable;
}
void setAutoBlock(JNIEnv* env, jobject obj){
    Entity entity = getPrimaryLocalPlayer();
    int y = EntityGetPosY(entity);
    float rotx = EntityGetRotX(entity);
    int toward = 1;
    if (rotx > 135.0f || rotx < -135.0f) {
        toward = 2;
    }
    if (rotx > -45.0f && rotx < 45.0f) {
        toward = 3;
    }
    if (rotx > 45.0f && rotx < 135.0f) {
        toward = 4;
    }
    if (rotx > -135.0f && rotx < -45.0f) {
        toward = 5;
    }
    if(!EntityisOnGround(thePlayer)){
        toward = 1;
        y = y - 1;
    }
    EntitySetAbility(entity, CANPUSHBLOCK, true);
    EntitySetBlockToward(entity, toward);
    EntitySetBlockX(entity, EntityGetPosX(entity));
    EntitySetBlockY(entity, y);
    EntitySetBlockZ(entity, EntityGetPosZ(entity));
}
void setFly(JNIEnv* env, jobject obj, bool enable){
    Fly_enable = enable;
}
void setCanFly(JNIEnv* env, jobject obj, bool enable){
    CanFly_enable = enable;
    EntitySetAbility(getPrimaryLocalPlayer(), FLYING, false);
}
void setJetPack(JNIEnv* env, jobject obj, bool enable, float speed){
    JetPack_enable = enable;
    JetPack_speed = speed;
}
void setbHop(JNIEnv* env, jobject obj, bool enable, float speed){
    bHop_enable = enable;
    bHop_speed = speed;
}
void setKillAura(JNIEnv* env, jobject obj, bool enable){
    KillAura_enable = enable;
}
void setHitBox(JNIEnv* env, jobject obj, bool enable, float width, float hight){
    HitBox_enable = enable;
    HitBox_width = width;
    HitBox_hight = hight;
}
void setTpAura(JNIEnv* env, jobject obj, bool enable, float x, float y, float z){
    TpAura_enable = enable;
    TpAura_x = x;
    TpAura_y = y;
    TpAura_z = z;
}
void setSurround(JNIEnv* env, jobject obj, bool enable, float r, float h, float speed){
    Surround_enable = enable;
    Surround_r = r;
    Surround_hight = h;
    Surround_speed = speed;
}
void setLockBack(JNIEnv* env, jobject obj, bool enable, float r, float h){
    LockBack_enable = enable;
    LockBack_r = r;
    LockBack_hight = h;
}
void setNoClip(JNIEnv* env, jobject obj, bool enable){
    NoClip_enable = enable;
}
void setClimb(JNIEnv* env, jobject obj, bool enable){
    Climb_enable = enable;
}
void setGameMode(JNIEnv* env, jobject obj, bool enable){
    GameMode_enable = enable;
}
bool PlayerIsMove(JNIEnv* env, jobject obj){
    return isMove(gameSetting);
}
long getLocalPlayer(JNIEnv* env, jobject obj){
    return getPrimaryLocalPlayer();
}
float getPlayerRotX(JNIEnv* env, jobject obj, long player){
    return EntityGetRotX(player);
}
float getPlayerRotY(JNIEnv* env, jobject obj, long player){
    return EntityGetRotY(player);
}
void hackTick(){
    while(true){
        if(thePlayer == 0 || !isPlayer(thePlayer)){
            thePlayer = getPrimaryLocalPlayer();
        }else{
            update(thePlayer);
            gameSetting = getGameSetting();
            if(isMove(gameSetting)){
                for(int keyCode = 0; keyCode < gameSetting.size(); keyCode++){
                    if(isDown(gameSetting[keyCode])){
                        onmove(gameSetting, keyCode, thePlayer);
                    }
                }
            }else{
                nomove(thePlayer);
            }
        }
        //std::this_thread::sleep_for(0.1s);
    }
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm,void *reserved);
jint JNICALL JNI_OnLoad(JavaVM *vm,void *reserved){
    //动态注册
    JNIEnv *env;
    if(vm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK) { return -1; }
    jclass JNICallBackClass = env->FindClass("com/XiaoHao/Module/Modules/JNICallBack");
    env->RegisterNatives(JNICallBackClass, ModuleCalled, sizeof(ModuleCalled) / sizeof(ModuleCalled[0]));
    return JNI_VERSION_1_6;
}

/*
int main(){
    pid_ = getPID("com.netease.x19");
    handle_ = getHandle(pid_);
    std::cout << pid_ << std::endl;
    std::cout << handle_ << std::endl;
    std::cout << std::hex << getPrimaryLocalPlayer() << std::endl;
    while(true){
        thePlayer = getPrimaryLocalPlayer();
    }
    return 0;
}
*/