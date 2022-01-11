#include"attack_text.h"

#include"game.h"
#include"utility.h"
#include"constants.h"

const float TEXT_SPEED = 30;
const int FONT_SIZE = 24;

void CreateAttackText(s_game* game, Vector2 position, const char* text, Color color){
    s_attack_text attack_text = {0};
    attack_text.position = position;
    attack_text.text = text;
    attack_text.color = color;
    attack_text.randomized_x_speed = GetRandomFloatValue(-TEXT_SPEED, TEXT_SPEED);
    attack_text.enabled = true;
    attack_text.t = 0;

    // Insert into game, look for available slot
    for(int i = 0; i < MAX_ATTACK_TEXT; i++){
        if(!game->attack_text[i].enabled){
            game->attack_text[i] = attack_text;
            break; // done no need to loop any further!
        }
    }
}

void UpdateAttackText(s_attack_text* attack_text){
    float dt = GetFrameTime();
    attack_text->t += 0.7f * dt;
    // Are we done?
    const float eps = 1e-2;
    if(attack_text->t >= 1-eps){
        attack_text->enabled = false;
    }
    // Update position on y and x axis
    attack_text->position.y -= TEXT_SPEED * dt;
    attack_text->position.x += attack_text->randomized_x_speed * dt;
}

void RenderAttackText(s_attack_text* attack_text){
    // Get a "alphalized" color from our t + text color value
    float a = 1 - attack_text->t;
    Color c = ColorAlpha(attack_text->color, a);
    DrawText(attack_text->text, attack_text->position.x, attack_text->position.y, FONT_SIZE, c);
    // DrawCircle(attack_text->position.x, attack_text->position.y, 32, c); // DEBUG
}