#pragma once
/**
 * @Author 小皓
 * @Date 2023/08/13 21:01 原创
 */
#include "Hack/Minecraft.h"
bool KillAura_enable = false,
    // AutoClick_enable = false,
    HitBox_enable = false,
    TpAura_enable = false,
    Surround_enable = false,
    LockBack_enable = false,
    AirJump_enable = false,
    Fly_enable = false,
    CanFly_enable = false,
    JetPack_enable = false,
    bHop_enable = false,
    // AutoBlock_enable = true,
    NoClip_enable = false,
    Climb_enable = false,
    GameMode_enable = false;

float bHop_speed = .45f;
float JetPack_speed = 1.f;
float HitBox_width = .6f, HitBox_hight = 1.8f, HitBox_range = 20.f;
float LockBack_r = 4.f, LockBack_hight = 2.5f;
float TpAura_x = .0f, TpAura_y = 1.8f, TpAura_z = .0f;
float Surround_r = 4.5f, Surround_hight = 2.5f, Surround_tick = 0, Surround_speed = 10.f;

void update(Entity entity);
void nomove(Entity entity);
void onmove(GameSetting gameSetting, KeyCode keyCode, Entity entity);
/* Combat */
void KillAura(Entity entity);
// void AutoClick(Entity entity);
void HitBox(Entity entity);
void TpAura(Entity entity);
void Surround(Entity entity);
void LockBack(Entity entity);

/* Movement */
void AirJump(Entity entity);
void Fly(Entity entity);
void CanFly(Entity entity);
void JetPack(Entity entity);
void bHop(GameSetting gameSetting, KeyCode key, Entity entity);
// void AutoBlock(GameSetting gameSetting, KeyCode key, Entity entity);

/* World */
void NoClip(Entity entity);
void Climb(Entity entity);
void GameMode(Entity entity);

void update(Entity entity){
    if(AirJump_enable){AirJump(entity);}
    if(Fly_enable){Fly(entity);}
    if(CanFly_enable){CanFly(entity);}
    if(JetPack_enable){JetPack(entity);}
    
    if(KillAura_enable){KillAura(entity);}
    // if(AutoClick_enable){AutoClick(entity);}
    if(HitBox_enable){HitBox(entity);}
    
    if(NoClip_enable){NoClip(entity);}
    if(Climb_enable){Climb(entity);}
    if(GameMode_enable){GameMode(entity);}
}
void nomove(Entity entity){
    if(TpAura_enable){TpAura(entity);}
    if(Surround_enable){Surround(entity);}
    if(LockBack_enable){LockBack(entity);}
}
void onmove(GameSetting gameSetting, KeyCode keyCode, Entity entity){
    if(bHop_enable){bHop(gameSetting, keyCode, entity);}
    // if(AutoBlock_enable){AutoBlock(gameSetting, keyCode, entity);}
}

void KillAura(Entity entity){
    EntitySetAbility(entity, SPRINTKILL, true);
}
/*
void AutoClick(Entity entity){
    system("input keyevent 'KEYCODE_Q'");
}*/
void HitBox(Entity entity){
    for(Entity tag : getEntitysNearEntity(entity)){
        if(fabs(EntityGetX(tag) - EntityGetX(entity)) < HitBox_range &&
           fabs(EntityGetY(tag) - EntityGetY(entity)) < HitBox_range &&
           fabs(EntityGetZ(tag) - EntityGetZ(entity)) < HitBox_range){
            EntitySetWidth(tag, HitBox_width);
            EntitySetHeight(tag, HitBox_hight);
        }
    }
}
void TpAura(Entity entity){
    Entitys entitys = getEntitysNearEntity(entity);
    if(entitys.size() > 0){
        sort(entitys.begin(), entitys.end(), EntitySort);
        if(sqrt(pow(EntityGetX(entity) - EntityGetX(entitys[0]), 2) + pow(EntityGetY(entity) - EntityGetY(entitys[0]), 2) + pow(EntityGetZ(entity) - EntityGetZ(entitys[0]), 2)) < 20){
            float motionx = (EntityGetX(entitys[0]) - EntityGetX(entity) + TpAura_x) * .45f;
            float motiony = (EntityGetY(entitys[0]) - EntityGetY(entity) + TpAura_y) * .45f;
            float motionz = (EntityGetZ(entitys[0]) - EntityGetZ(entity) + TpAura_z) * .45f;
            EntitySetMotionX(entity, motionx);
            EntitySetMotionY(entity, motiony);
            EntitySetMotionZ(entity, motionz);
        }
    }
}
void Surround(Entity entity){
    Entitys entitys = getEntitysNearEntity(entity);
    if(entitys.size() > 0){
        sort(entitys.begin(), entitys.end(), EntitySort);
        if(sqrt(pow(EntityGetX(entity) - EntityGetX(entitys[0]), 2) + pow(EntityGetY(entity) - EntityGetY(entitys[0]), 2) + pow(EntityGetZ(entity) - EntityGetZ(entitys[0]), 2)) < 20){
            float motionx = ((EntityGetX(entitys[0]) + Surround_r * sin(0.01745f * Surround_tick)) - EntityGetX(entity)) * .45f;
            float motiony = (EntityGetY(entitys[0]) - EntityGetY(entity) + Surround_hight) * .45f;
            float motionz = ((EntityGetZ(entitys[0]) + Surround_r * cos(0.01745f * Surround_tick)) - EntityGetZ(entity)) * .45f;
            EntitySetMotionX(entity, motionx);
            EntitySetMotionY(entity, motiony);
            EntitySetMotionZ(entity, motionz);
            Surround_tick = Surround_tick > 360.f ? .0f : Surround_tick + Surround_speed;
        }
    }
}
void LockBack(Entity entity){
    Entitys entitys = getEntitysNearEntity(entity);
    if(entitys.size() > 0){
        sort(entitys.begin(), entitys.end(), EntitySort);
        if(sqrt(pow(EntityGetX(entity) - EntityGetX(entitys[0]), 2) + pow(EntityGetY(entity) - EntityGetY(entitys[0]), 2) + pow(EntityGetZ(entity) - EntityGetZ(entitys[0]), 2)) < 20){
            float rotx = EntityGetRotX(entitys[0]);
            float motionx = ((EntityGetX(entitys[0]) + LockBack_r * cos((rotx-90)*3.1415926/180)) - EntityGetX(entity) ) * .45f;
            float motiony = (EntityGetY(entitys[0]) - EntityGetY(entity) + LockBack_hight) * .45f;
            float motionz = ((EntityGetZ(entitys[0]) + LockBack_r * sin((rotx-90)*3.1415926/180)) - EntityGetZ(entity) ) * .45f;
            EntitySetMotionX(entity, motionx);
            EntitySetMotionY(entity, motiony);
            EntitySetMotionZ(entity, motionz);
        }
    }
}
void AirJump(Entity entity){
    EntitySetAbility(entity, JUMP, true);
}
void Fly(Entity entity){
    EntitySetAbility(entity, FLYING, true);
}
void CanFly(Entity entity){
    EntitySetAbility(entity, CANFLY, true);
}
void JetPack(Entity entity){
    float rotx = EntityGetRotX(entity);
    float roty = EntityGetRotY(entity);
    EntitySetMotionX(entity, JetPack_speed * -sin(rad((rotx < 0) ? rotx + 360.f : rotx)) * cos(rad(-roty)));
    EntitySetMotionY(entity, sin(rad(-roty)) * JetPack_speed);
    EntitySetMotionZ(entity, JetPack_speed * cos(rad((rotx < 0) ? rotx + 360 : rotx)) * cos(rad(-roty)));
}
void bHop(GameSetting gameSetting, KeyCode key, Entity entity){
    if(EntityisOnGround(entity)) EntitySetMotionY(entity, bHop_speed);
    switch(key){
        case Head:
            EntitySetMotionX(entity, -sin(rad(EntityGetRotX(entity))) * bHop_speed);
            EntitySetMotionZ(entity, cos(rad(EntityGetRotX(entity))) * bHop_speed);
            break;
        case Back:
            EntitySetMotionX(entity, sin(rad(EntityGetRotX(entity))) * bHop_speed);
            EntitySetMotionZ(entity, -cos(rad(EntityGetRotX(entity))) * bHop_speed);
            break;
        case Left:
            EntitySetMotionX(entity, -sin(rad(EntityGetRotX(entity) - 90.0f)) * bHop_speed);
            EntitySetMotionZ(entity, cos(rad(EntityGetRotX(entity) - 90.0f)) * bHop_speed);
            break;
        case Right:
            EntitySetMotionX(entity, -sin(rad(EntityGetRotX(entity) + 90.0f)) * bHop_speed);
            EntitySetMotionZ(entity, cos(rad(EntityGetRotX(entity) + 90.0f)) * bHop_speed);
            break;
        case HeadLeft:
            EntitySetMotionX(entity, -sin(rad(EntityGetRotX(entity) - 45.0f)) * bHop_speed);
            EntitySetMotionZ(entity, cos(rad(EntityGetRotX(entity) - 45.0f)) * bHop_speed);
            break;
        case HeadRight:
            EntitySetMotionX(entity, -sin(rad(EntityGetRotX(entity) + 45.0f)) * bHop_speed);
            EntitySetMotionZ(entity, cos(rad(EntityGetRotX(entity) + 45.0f)) * bHop_speed);
            break;
        default:
            break;
    }
}
/*
void AutoBlock(GameSetting gameSetting, KeyCode key, Entity entity){
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
    if(!EntityisOnGround(entity)){
        toward = 1;
    }
    EntitySetAbility(entity, CANPUSHBLOCK, true);
    EntitySetBlockToward(entity, toward);
    EntitySetBlockX(entity, EntityGetPosX(entity));
    EntitySetBlockY(entity, EntityGetPosY(entity));
    EntitySetBlockZ(entity, EntityGetPosZ(entity));
    system("input keyevent 'KEYCODE_E'");
}*/
void NoClip(Entity entity){
    EntitySetAbility(entity, NOCLIP, true);
}
void Climb(Entity entity){
    EntitySetAbility(entity, CLIMB, true);
}
void GameMode(Entity entity){
    EntitySetAbility(entity, GAMEMODE, true);
}