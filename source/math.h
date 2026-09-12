
#define GRAVITY 1
#define MAX_FALL_SPEED 5
#define GROUND_Y 120
#define ACELERATION 1
#define MAX_SPEED 3
#define JUMP_FORCE -10
#define JUMPS 2
#define MAX_BULLET_DISTANCE 150
#define IMPUSE_FORCE 2


int validarColision(Sprite *a, Sprite *b)
{
    return a->x < b->x + b->width &&
           a->x + a->width > b->x &&
           a->y < b->y + b->height &&
           a->y + a->height > b->y;
}

void impulseProjectil(Sprite *a)
{
    a->vx = IMPUSE_FORCE;
    a->x += a->vx;
    a->distance += IMPUSE_FORCE;
    if(a->distance > MAX_BULLET_DISTANCE)
    {
        a->distance = 0;
        a->active = 0;
        obj_hide(a->oam);
    }

}

void caidaLibre(Sprite *a)
{
    a->vy += GRAVITY;
    if(a->vy > MAX_FALL_SPEED){
        a->vy = MAX_FALL_SPEED;
    }
    a->y += a->vy;
    if(a->y + a->height >= GROUND_Y)
    {
        a->y = GROUND_Y - a->height;
        a->vy = 0;
        a->hasGrounded = 1;
        a->doubleJump = JUMPS;
    }else{
        a->hasGrounded = 0;
    }
}

void salto(Sprite *a)
{
    a->vy = JUMP_FORCE;
    a->doubleJump--;
    a->hasGrounded = 0;
}

void movimiento(Sprite *a)
{

}

