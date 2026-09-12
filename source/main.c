
#include <tonc.h>
#include <maxmod.h>
#include "thekid.h"
#include "coin.h"
#include "Bullet.h"
#include "background.h"
#include "soundbank.h"
#include "soundbank_bin.h"

#define MAX_PROJECTILE 20
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

typedef enum
{
    OBJECT_PLAYER,
    OBJECT_COIN,
    OBJECT_PROJECTIL
} objectType;

typedef struct 
{
    /* data */
    OBJ_ATTR *oam;
    objectType typeObject;
    int x;
    int y;
    int vx;
    int vy;
    int tile;
    int palette;
    int width;
    int height;
    int tileStart;
    int spriteCount;
    int frame;
    int frameTime;
    int active;
    int hasGravity;
    int hasGrounded;
    int doubleJump;
    int distance;
} Sprite;

Sprite sprites[2];


Sprite projectile[MAX_PROJECTILE];

//Declaro al personaje jugable de todos los sprites existentes
Sprite *player;

OBJ_AFFINE affineMatrices[1];

int indexBullet;

int screenX = 0, screenY = 0;

#include "math.h"

#pragma region Inicialización de los sprites
void spriteInit(Sprite *sprite, int oamIndex, int tile, int palette, int spriteCount, int x, int y, int width, int height, objectType type, int HG, int activo)
{
    sprite->oam = &oam_mem[oamIndex];
    sprite->tile = tile;
    sprite->palette = palette;
    sprite->tileStart = tile;
    sprite->frame = 0;
    sprite->frameTime = 0;
    sprite->spriteCount = spriteCount;
    sprite->x = x;
    sprite->y = y;
    sprite->vx = 0;
    sprite->vy = 0;
    sprite->width = width;
    sprite->height = height;
    sprite->active = activo;
    sprite->typeObject = type;
    sprite->hasGravity = HG;
    sprite->hasGrounded = 0;
    sprite->doubleJump = 0;
    sprite->distance = 0;
}

void projectilLoad(Sprite *bullet)
{
    bullet->x = player->x + 25;
    bullet->y = player->y + 17;
    bullet->active = 1;
}


void spriteLoad(Sprite *sprite)
{
    obj_set_attr(sprite->oam, ATTR0_SQUARE | ATTR0_8BPP, ATTR1_SIZE_32x32, ATTR2_ID(sprite->tile));
    obj_set_pos(sprite->oam, sprite->x, sprite->y);
}

void projectiLoad(Sprite *sprite)
{
    obj_set_attr(sprite->oam, ATTR0_SQUARE | ATTR0_8BPP, ATTR1_SIZE_8x8, ATTR2_ID(sprite->tile));
    obj_set_pos(sprite->oam, sprite->x, sprite->y);
}

void spriteSetPosition(Sprite *sprite, int x, int y){
    sprite->x = x;
    sprite->y = y;
}

void spriteMove(Sprite *sprite, int dx, int dy){
    sprite->x += dx;
    sprite->y += dy;
}

void spriteAnimation(Sprite *sprite, int spriteSize)
{
    if(sprite->spriteCount <= 1)
    {
        return;
    }
    sprite->frameTime++;
    if(sprite->frameTime >= 10)
    {
        sprite->frameTime = 0;
        sprite->frame++;
        if(sprite->frame >= sprite->spriteCount)
        {
            sprite->frame = 0;
        }
        sprite->tile = sprite->tileStart + (sprite->frame * spriteSize);
    }
}
#pragma endregion

void draw(void)
{
    for(int i = 0; i < ARRAY_SIZE(sprites); i++)
    {
        Sprite *sprite = &sprites[i];
        if(sprite->active == 0){
             obj_hide(sprite->oam);
             continue;
        }
        if(sprite->typeObject == OBJECT_COIN)
        {
            if(validarColision(player, &sprites[i]) == 1)
            {
                sprite->active = 0;
            }
        }
        if(sprite->hasGravity == 1 && sprite->hasGrounded == 0)
        {
            caidaLibre(&sprites[i]);
        }
        spriteAnimation(&sprites[i], 32);
        spriteLoad(&sprites[i]);
    }
    for(int i = 0; i < ARRAY_SIZE(projectile); i++){
        Sprite *sprite = &projectile[i];
        if(sprite->active == 1)
        {
            spriteAnimation(&projectile[i], 2);
            projectiLoad(&projectile[i]);
            impulseProjectil(&projectile[i]);
        }
    }
}

void update(void)
{
    if(key_is_down(KEY_LEFT))
    {
        spriteMove(player,-1,0);
        screenX -= 2;
    }
    if(key_is_down(KEY_RIGHT))
    {
        spriteMove(player,1,0);
        screenX += 2;
    }
    if(key_hit(KEY_B) && player->doubleJump > 0){
        if(player->doubleJump == 1)   
            mmEffect(SFX_JUMP);
        if(player->doubleJump == 2)   
            mmEffect(SFX_DOUBLEJUMP);
        salto(player);
    }
    if(key_hit(KEY_A))
    {
        mmEffect(SFX_SHOOT);
        projectilLoad(&projectile[indexBullet]);
        indexBullet++;
        if(indexBullet >= MAX_PROJECTILE)
        {
            indexBullet = 0;
        }
    }
}

int main(void)
{

    irq_init(NULL);
    irq_set(II_VBLANK, mmVBlank, 0);
    irq_enable(II_VBLANK);
    oam_init(oam_mem, 128);
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
    mmInitDefault((mm_addr)soundbank_bin, 8);
    mmStart(MOD_11THHOUR, MM_PLAY_LOOP);

   
   
    //inicializamos el sprite de la caja
    memcpy32(tile_mem_obj[0], thekidTiles, thekidTilesLen / 4);
    memcpy16(pal_obj_mem, thekidPal,thekidPalLen / 2);
    spriteInit(&sprites[0], 0, 0, 0, 4, 60, 60,32,32, OBJECT_PLAYER,1,1);


    //Asigno cual sprite de la lista es mi player
    memcpy32(tile_mem_obj[0] + 128, BulletTiles, BulletTilesLen / 4);
    int y = 1;
    for(int x = 0; x < MAX_PROJECTILE; x++){
        spriteInit(&projectile[x], y, 128, 1, 2, 100, 80,8,8, OBJECT_PROJECTIL,0,0);
        y++;
    }
    // spriteInit(&projectile[1], 2, 128, 1, 2, 100, 90,8,8, OBJECT_PROJECTIL,0);
    // memcpy32()
    player = &sprites[0];
    indexBullet = 0;
    //inicializamos el background
    memcpy16(pal_bg_mem, backgroundPal, backgroundPalLen / 2);
    memcpy32(tile_mem[0], backgroundTiles, backgroundTilesLen / 4);
    memcpy16(se_mem[31], 0, 32 * 32 / 2);
    for (int y = 0; y < 20; y++)
    {
        for (int x = 0; x < 30; x++)
        {
            se_mem[31][y * 32 + x] =
                backgroundMap[y * 30 + x];
        }
    }
    REG_BG0CNT = BG_CBB(0) | BG_SBB(31) | BG_8BPP | BG_REG_32x32;
    
  
    while(1){
        key_poll();
        update();
        mmFrame();
        REG_BG0HOFS = screenX;
        REG_BG0VOFS = screenY;
        VBlankIntrWait();
        draw();

    }
    return 0;
}

