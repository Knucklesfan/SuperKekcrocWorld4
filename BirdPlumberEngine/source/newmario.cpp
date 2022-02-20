#include "newmario.h"

	void MPlayer::Die() {
		if (PlayerControlled) {
			if (!DEAD) {
				X_SPEED = 0; Y_SPEED = 0; STATE = 0;
				//RAM[0x1DFB] = 9; PLAY SOUND EFFECT
				DEATH_TIMER = y < -16.0 ? 150 : 32;
				DEAD = true;
			}
		}
	}
bool MPlayer::Hurt() {
		if (INVINCIBILITY_FRAMES_STAR > 0) {
			return false;
		}
		if (!DEAD && INVINCIBILITY_FRAMES == 0) {
			if (STATE != 0) {
				STATE = 0;
				powerup_anim = 0xDF;
				INVINCIBILITY_FRAMES = 90;
				//RAM[0x1DF9] = 4;
				//if (!networking) { RAM[0x9D] = 0; }
			}
			else {
				Die();
				return true;
			}
		}
		return false;
	};
void MPlayer::getInput() {
			//TO-DO: Modularize local multiplayer behaviour.
            const Uint8* state = SDL_GetKeyboardState(NULL);
			p_pad[button_y] = state[SDL_SCANCODE_A] || state[SDL_SCANCODE_S];
			p_pad[button_b] = state[SDL_SCANCODE_Z];
			p_pad[button_a] = state[SDL_SCANCODE_X];
			p_pad[button_left] = state[SDL_SCANCODE_LEFT];
			p_pad[button_right] = state[SDL_SCANCODE_RIGHT];
			p_pad[button_down] = state[SDL_SCANCODE_DOWN];
			p_pad[button_up] = state[SDL_SCANCODE_UP];
			p_pad[button_start] = state[SDL_SCANCODE_RETURN];
}
void MPlayer::Enemy_Jump()  {
		if (enemyjump_cooldown) { return; }
		enemyjump_cooldown = 8;

		score_streak++;
		if (!isClient) {
			addScore(uint_fast32_t(score_streak) * 200);
		}

		createParticle(0x44, 0x11, 0x8, 5, x, y - 24.0, 0, 0, 0);
		
		if (!jump_is_spin) {
			if (p_pad[button_b] || p_pad[button_a]) {
				Y_SPEED = Calculate_Speed(1408);
			}
			else {
				Y_SPEED = Calculate_Speed(800);
			}
		}
	}