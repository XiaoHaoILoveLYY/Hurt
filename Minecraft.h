#pragma once
/**
 * @Author 小皓
 * @Date 2023/08/13 21:01 原创
 */
#include <iostream>
#include "Utils.h"
#define Entity Addr
#define KeyPointer Addr
#define EntityAbility int
#define KeyCode int
#define GameSetting std::vector<KeyPointer>
#define Entitys std::vector<Entity>

enum Ability {
    CANFLY,
    FLYING,
    JUMP,
    CLIMB,
    NOCLIP,
    GROUND,
    GAMEMODE,
    SPRINTKILL,
    CANPUSHBLOCK
};

enum Keys {
    Jump,
    Head,
    Back,
    Left,
    Right,
    HeadLeft,
    HeadRight
};

Entity getPrimaryLocalPlayer();
GameSetting getGameSetting();
Entitys getEntitysNearEntity(Entity entity);
bool isDown(KeyPointer key);
bool isMove(GameSetting gameSetting);
bool isPlayer(Entity entity);
bool EntityisOnGround(Entity entity);
bool EntityisFlying(Entity entity);
bool EntityisCanFly(Entity entity);
bool EntityisCanJump(Entity entity);
bool EntityisNoClip(Entity entity);
bool EntityisSprintKill(Entity entity);
float EntityGetWidth(Entity entity);
float EntityGetHeight(Entity entity);
float EntityGetRotX(Entity entity);
float EntityGetRotY(Entity entity);
float EntityGetX(Entity entity);
float EntityGetY(Entity entity);
float EntityGetZ(Entity entity);
float EntityGetMotionX(Entity entity);
float EntityGetMotionY(Entity entity);
float EntityGetMotionZ(Entity entity);
float EntityGetBoxX(Entity entity);
float EntityGetBoxY(Entity entity);
float EntityGetBoxZ(Entity entity);
int EntityGetPosX(Entity entity);
int EntityGetPosY(Entity entity);
int EntityGetPosZ(Entity entity);
int EntityGetBlockX(Entity entity);
int EntityGetBlockY(Entity entity);
int EntityGetBlockZ(Entity entity);
int EntityGetBlockToward(Entity entity);
void EntitySetAbility(Entity entity, EntityAbility ability, bool enable);
void EntitySetWidth(Entity entity, float width);
void EntitySetHeight(Entity entity, float height);
void EntitySetRotX(Entity entity, float x);
void EntitySetRotY(Entity entity, float y);
void EntitySetBoxX(Entity entity, float x);
void EntitySetBoxY(Entity entity, float y);
void EntitySetBoxZ(Entity entity, float z);
void EntitySetMotionX(Entity entity, float x);
void EntitySetMotionY(Entity entity, float y);
void EntitySetMotionZ(Entity entity, float z);
void EntitySetBlockX(Entity entity, int x);
void EntitySetBlockY(Entity entity, int y);
void EntitySetBlockZ(Entity entity, int z);
void EntitySetBlockToward(Entity entity, int toward);
bool EntitySort(Entity entity1, Entity entity2);

Entity getPrimaryLocalPlayer(){
    Entity tag = getPointerAddr("libminecraftpe.so:bss -> 0xE79C8BE4BDA0E5A688E79A84E5818FE7A7BB -> 0xE79C8BE4BDA0E5A688E79A84E5818FE7A7BB -> 0xE79C8BE4BDA0E5A688E79A84E5818FE7A7BB -> 0xE79C8BE4BDA0E5A688E79A84E5818FE7A7BB");
    Entity temp = getAddrQword(tag + 0xE79C8BE4BDA0E5A688E79A84E5818FE7A7BB);
    if(temp == 0 && isPlayer(tag)) {
        return tag;
    }
    if(temp != 0 && isPlayer(temp)) {
        return temp;
    }
    return 0;
}
GameSetting getGameSetting(){
    GameSetting gameSetting;
    KeyPointer keyPointer = getPointerAddr("libminecraftpe.so:bss -> 0xE79C8BE4BDA0E5A688E79A84E5818FE7A7BB -> 0xE79C8BE4BDA0E5A688E79A84E5818FE7A7BB -> 0xE79C8BE4BDA0E5A688E79A84E5818FE7A7BB -> 0xE79C8BE4BDA0E5A688E79A84E5818FE7A7BB");
    gameSetting.push_back(keyPointer);
    for(int i = 4; i < 10; i++) {
        gameSetting.push_back(keyPointer + i);
    }
    return gameSetting;
}
Entitys getEntitysNearEntity(Entity entity){
    Entitys entitys;
    Addr memPage = getPointerAddr(entity, {0xE79C8BE4BDA0E5A688E79A84E5818FE7A7BB, 0xE79C8BE4BDA0E5A688E79A84E5818FE7A7BB, 0xE79C8BE4BDA0E5A688E79A84E5818FE7A7BB});
    for(int i = 0; i < 300; i++) {
        Entity tagEntity = getAddrQword(memPage + 0x8 * i);
        if(tagEntity == 0){ break; }
        else if(tagEntity == entity){ continue; }
        else if(isPlayer(tagEntity)){ entitys.push_back(tagEntity); }
        else{ break; }
    }
    return entitys;
}
bool isDown(KeyPointer key){
    return getAddrByte(key) == 1;
}
bool isMove(GameSetting gameSetting){
    for(KeyPointer key : gameSetting) {
        if(isDown(key))
            return true;
    }
    return false;
}
bool isPlayer(Entity entity){
    return getAddrByte(entity + 0xE79C8BE4BDA0E5A688E79A84E5818FE7A7BB) == 1;
}
bool EntityisOnGround(Entity entity){
    return getAddrByte(entity + 466) == 1;
}
bool EntityisFlying(Entity entity){
    return getAddrByte(entity + 3432) == 1;
}
bool EntityisCanFly(Entity entity){
    return getAddrByte(entity + 3444) == 1;
}
bool EntityisCanJump(Entity entity){
    return getAddrByte(entity + 456) == 1;
}
bool EntityisNoClip(Entity entity){
    return getAddrByte(entity + 3522) == 1;
}
bool EntityisSprintKill(Entity entity){
    return getAddrDword(getPointerAddr(entity, {0xE79C8BE4BDA0E5A688E79A84E5818FE7A7BB, 0xE79C8BE4BDA0E5A688E79A84E5818FE7A7BB, 0xE79C8BE4BDA0E5A688E79A84E5818FE7A7BB})) != 98312;
}
float EntityGetWidth(Entity entity){
    return getAddrFloat(entity + 1264);
}
float EntityGetHeight(Entity entity){
    return getAddrFloat(entity + 1268);
}
float EntityGetRotX(Entity entity){
    return getAddrFloat(entity + 300);
}
float EntityGetRotY(Entity entity){
    return getAddrFloat(entity + 296);
}
float EntityGetX(Entity entity){
    return getAddrFloat(entity + 1272);
}
float EntityGetY(Entity entity){
    return getAddrFloat(entity + 1276);
}
float EntityGetZ(Entity entity){
    return getAddrFloat(entity + 1280);
}
float EntityGetMotionX(Entity entity){
    return getAddrFloat(entity + 1296);
}
float EntityGetMotionY(Entity entity){
    return getAddrFloat(entity + 1300);
}
float EntityGetMotionZ(Entity entity){
    return getAddrFloat(entity + 1304);
}
float EntityGetBoxX(Entity entity){
    return getAddrFloat(entity + 1284);
}
float EntityGetBoxY(Entity entity){
    return getAddrFloat(entity + 1288);
}
float EntityGetBoxZ(Entity entity){
    return getAddrFloat(entity + 1292);
}
int EntityGetPosX(Entity entity){
    return getAddrDword(entity + 0x3D0);
}
int EntityGetPosY(Entity entity){
    return getAddrDword(entity + 0x3D4);
}
int EntityGetPosZ(Entity entity){
    return getAddrDword(entity + 0x3D8);
}
int EntityGetBlockX(Entity entity){
    return getAddrDword(getPointerAddr(entity, {928, 3220}) + 0x14);
}
int EntityGetBlockY(Entity entity){
    return getAddrDword(getPointerAddr(entity, {928, 3220}) + 0x18);
}
int EntityGetBlockZ(Entity entity){
    return getAddrDword(getPointerAddr(entity, {928, 3220}) + 0x1C);
}
int EntityGetBlockToward(Entity entity){
    return getAddrDword(getPointerAddr(entity, {928, 3220}) + 0x10);
}
void EntitySetAbility(Entity entity, EntityAbility ability, bool enable){
    switch(ability) {
        case CANFLY:
            setAddrByte(entity + 3444, enable ? 1 : 0);
        break;
        case FLYING:
            setAddrByte(entity + 3432, enable ? 1 : 0);
        break;
        case JUMP:
            setAddrByte(entity + 456, enable ? 1 : 0);
        break;
        case CLIMB:
            setAddrFloat(entity + 564, enable ? 1000.f : -1.f);
        break;
        case NOCLIP:
            setAddrByte(entity + 3522, enable ? 1 : 0);
        break;
        case GROUND:
            setAddrByte(entity + 466, enable ? 1 : 0);
        break;
        case GAMEMODE:
            setAddrByte(entity + 3809376, enable ? 1 : 0);
        break;
        case SPRINTKILL:
            setAddrDword(getPointerAddr(entity, {0x168, 0x0, 0x14}), enable ? 8486920 : 98312);
        break;
        case CANPUSHBLOCK:
            setAddrDword(getPointerAddr(entity, {928, 3220}) + 0xC, 0);
            setAddrFloat(getPointerAddr(entity, {928, 3220}), 0);
            setAddrFloat(getPointerAddr(entity, {928, 3220}) + 0x4, -2.f);
            setAddrFloat(getPointerAddr(entity, {928, 3220}) + 0x8, 0);
        break;
        default:
        break;
    }
}
void EntitySetWidth(Entity entity, float width){
    setAddrFloat(entity + 1264, width);
}
void EntitySetHeight(Entity entity, float height){
    setAddrFloat(entity + 1268, height);
}
void EntitySetRotX(Entity entity, float x){
    setAddrFloat(entity + 300, x);
}
void EntitySetRotY(Entity entity, float y){
    setAddrFloat(entity + 296, y);
}
void EntitySetBoxX(Entity entity, float x){
    setAddrFloat(entity + 1284, x);
}
void EntitySetBoxY(Entity entity, float y){
    setAddrFloat(entity + 1288, y);
}
void EntitySetBoxZ(Entity entity, float z){
    setAddrFloat(entity + 1292, z);
}
void EntitySetMotionX(Entity entity, float x){
    setAddrFloat(entity + 1296, x);
}
void EntitySetMotionY(Entity entity, float y){
    setAddrFloat(entity + 1300, y);
}
void EntitySetMotionZ(Entity entity, float z){
    setAddrFloat(entity + 1304, z);
}
void EntitySetBlockX(Entity entity, int x){
    setAddrDword(getPointerAddr(entity, {928, 3220}) + 0x14, x);
}
void EntitySetBlockY(Entity entity, int y){
    setAddrDword(getPointerAddr(entity, {928, 3220}) + 0x18, y);
}
void EntitySetBlockZ(Entity entity, int z){
    setAddrDword(getPointerAddr(entity, {928, 3220}) + 0x1C, z);
}
void EntitySetBlockToward(Entity entity, int toward){
    setAddrDword(getPointerAddr(entity, {928, 3220}) + 0x10, toward);
}
bool EntitySort(Entity entity1, Entity entity2){
    Entity entity = getPrimaryLocalPlayer();
    
    float x = EntityGetX(entity);
    float y = EntityGetY(entity);
    float z = EntityGetZ(entity);
    
    float ax = EntityGetX(entity1);
    float ay = EntityGetY(entity1);
    float az = EntityGetZ(entity1);
    
    float bx = EntityGetX(entity2);
    float by = EntityGetY(entity2);
    float bz = EntityGetZ(entity2);
    
    float d1 = sqrt(pow(x - ax, 2) + pow(y - ay, 2) + pow(z - az, 2));
    float d2 = sqrt(pow(x - bx, 2) + pow(y - by, 2) + pow(z - bz, 2));
    
    return d1 < d2;
}
