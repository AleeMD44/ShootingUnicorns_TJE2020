#include "Stage.h"
#include "game.h"

//Clase Stage --> Almacena los diferentes Stages que tiene el juego.
Sprite* bullets[20];
Sprite* enemies[10];
Sprite* finalboss;

void Stage::render(Image& fb) {}

//Funcion para cambiar el Stage segun el nombre.
void Stage::changeStage(const char * name)
{
	current_stage = s_stages[name];
}

Stage::Stage(const char* name) {s_stages[name] = this; if (current_stage == NULL) current_stage = this;}

Stage* Stage::current_stage = NULL;
std::map<std::string, Stage*> Stage::s_stages;

void Stage::update(double seconds_elapsed){}

void PlayStage::render(Image& fb) {

	//Renderizamos el mapa
	Game::instance->world->map->render(fb, Game::instance->world->tileset);
	//Pintamos en el framebuffer el jugador.
	fb.drawImage(Game::instance->world->tileset, Game::instance->world->player->player_pos.x, Game::instance->world->player->player_pos.y, Area(16 + 8 * Game::instance->world->player->facing, 8, 8, 8));

	//Si estamos en el 3r PlayStage...
	if (counter == 2) {

		if (bullets[Game::instance->world->player->numBullets] != NULL) {
			if (bullets[Game::instance->world->player->numBullets]->shoot && Game::instance->world->player->numBullets > 0) {
				fb.drawImage(Game::instance->world->tileset, bullets[Game::instance->world->player->numBullets]->player_pos.x, bullets[Game::instance->world->player->numBullets]->player_pos.y, Area(7 * 8, 8, 8, 8));
			}
		}

		//Pintamos la información sobre las balas y el reinicio.
		fb.drawText("Bullets: " + toString(Game::instance->world->player->numBullets) + " / 20", 8, 2, Game::instance->world->minifontw, 4, 6);
		fb.drawText("Press R to restart", 80, 2, Game::instance->world->minifontw, 4, 6);

		//Si hay enemigos los pintamos.
		if (nenemies > 0) {
			for (int i = 0; i < nenemies; i++) {
				if (enemies[i] != NULL) {
					fb.drawImage(Game::instance->world->tileset, enemies[i]->player_pos.x, enemies[i]->player_pos.y, Area(16 + 8 * enemies[i]->facing, 16, 8, 8));
				}
			}
		}
	}

	if (counter == 3) {
		if (round == 1) {
			fb.drawText("Nothing in here", fb.width / 2 - 25, fb.height / 2, Game::instance->world->minifontw, 4, 6);
		}
		else if (round == 2) {
			fb.drawText("Nothing in here either", fb.width / 2 - 35, fb.height / 2, Game::instance->world->minifontw, 4, 6);
		}else if (round == 3) {
			fb.drawText("You really came here for hothing?", fb.width / 2 - 60, fb.height / 2, Game::instance->world->minifontw, 4, 6);
		}
	}

	if (counter == 4) {

		if (bullets[Game::instance->world->player->numBullets] != NULL) {
			if (bullets[Game::instance->world->player->numBullets]->shoot && Game::instance->world->player->numBullets > 0) {
				fb.drawImage(Game::instance->world->tileset, bullets[Game::instance->world->player->numBullets]->player_pos.x, bullets[Game::instance->world->player->numBullets]->player_pos.y, Area(7 * 8, 8, 8, 8));
			}
		}

		fb.drawText("Bullets: " + toString(Game::instance->world->player->numBullets) + " / 20", 8, 2, Game::instance->world->minifontw, 4, 6);
		fb.drawText("Press R to restart", 80, 2, Game::instance->world->minifontw, 4, 6);
		if(finalboss != NULL)
			fb.drawImage(Game::instance->world->tileset, finalboss->player_pos.x, finalboss->player_pos.y, Area(16 + 8 * finalboss->facing, 4*8, 8, 8));
	}


}

void PlayStage::update(double seconds_elapsed)
{

	//Si se tienen las pistolas y las balas, comprobamos si existe la bala y si no la creamos. Una vez creada, si presionamos ESPACIO podremos disparar.
	if (Game::instance->world->player->shoot && Game::instance->world->player->numBullets > 0) {
		if(bullets[Game::instance->world->player->numBullets] == NULL)
			bullets[Game::instance->world->player->numBullets] = new Sprite();

		if (Input::wasKeyPressed(SDL_SCANCODE_SPACE) && bullets[Game::instance->world->player->numBullets]->shoot == false && Game::instance->world->player->numBullets > 0) {
			bullets[Game::instance->world->player->numBullets]->player_pos = Game::instance->world->player->player_pos;
			bullets[Game::instance->world->player->numBullets]->facing = Game::instance->world->player->facing;
			bullets[Game::instance->world->player->numBullets]->shoot = true;
		}

	}
	//Segun la tecla que pulsemos nos moveremos en X direccion.
	if (Input::wasKeyPressed(SDL_SCANCODE_UP)) //if key up
	{
			Game::instance->world->player->Move(FACE_UP);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{
			Game::instance->world->player->Move(FACE_DOWN);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_RIGHT)) //if key rigbt
	{
			Game::instance->world->player->Move(FACE_RIGHT);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_LEFT)) //if key left
	{
			Game::instance->world->player->Move(FACE_LEFT);
	}
	//Si pulsamos escape volvemos al Menu
	if (Input::wasKeyPressed(SDL_SCANCODE_ESCAPE)) //if key left
	{
		restart("data/menu.map");
		changeStage("menu");
	}
	//Si pulsamos R reiniciamos la partida
	if (Input::wasKeyPressed(SDL_SCANCODE_R)) {
		restart("data/intro.map");
	}

	//Depende de en que PlayStage estemos realizamos unas opciones u otras.
	if (counter == 0 || counter == 1) {
		//Comprobamos si estamos sobre la pistola o sobre la bala y activamos sus atributos.
		if (Game::instance->world->map->isObject(Game::instance->world->player->player_pos, GUNB, GROUNDB)) {
			Game::instance->world->player->shoot = true;
		}
		if (Game::instance->world->map->isObject(Game::instance->world->player->player_pos, BULLETB, GROUNDB)) {

			Game::instance->world->player->numBullets = 3;
		}
		if (counter == 0) {
			//Si tenemos balas podemos pasar a la siguiente PlayStage
			if(Game::instance->world->player->numBullets > 0){
				Game::instance->world->map->changeTile(Vector2(20, 8), EXIT);
				Game::instance->world->map->changeTile(Vector2(19, 8), ARROWRIGHTB);
			}
		}
		else {
			//Si tenemos la pistola podemos pasar a la siguiente PlayStage
			if (Game::instance->world->player->shoot) {
				Game::instance->world->map->changeTile(Vector2(20, 8), EXIT);
				Game::instance->world->map->changeTile(Vector2(19, 8), ARROWRIGHTB);
			}
		}
		//Si estamos sobre la salida, cambiamos el PlayStage (cambiando el mapa y la posición del jugador).
		if (Game::instance->world->map->isObject(Game::instance->world->player->player_pos, EXIT, EMPTY)) {
			if (counter == 0) {
				Game::instance->world->map->loadGameMap("data/intro2.map");
				Game::instance->world->player->player_pos = Vector2(1 * 8, 8 * 8);
			}
			else{
				Game::instance->world->map->loadGameMap("data/play1.map");
				Game::instance->world->player->player_pos = Vector2(1 * 8, 8 * 8);
			}
			//Aumentamos el contador que nos dice en que PlayStage nos encontramos.
			counter++;
		}
	}else if(counter == 2){

		if (Input::wasKeyPressed(SDL_SCANCODE_Z)) {
			round = 4;
			for (int i = 0; i < nenemies; i++) {
				if (enemies[i] == NULL) continue;
				enemies[i] = NULL;
			}
		}


		//Si no hay enemigos en pantalla los creamos
		if (enemiesinscreen == 0) {
			if (round < 3)
				round++;
			else
				round = 4;

			if (round == 1) {
				generateEnemies(3);
			}
			else if (round == 2) {
				if (Game::instance->world->player->player_pos.x <= 10*8) {
					Game::instance->world->map->changeTile(Vector2(13, 2), PLAYER);
				}
				else {
					Game::instance->world->map->changeTile(Vector2(2, 2), PLAYER);
				}
				generateEnemies(5);
			}
			else if (round == 3) {
				if (Game::instance->world->player->player_pos.x <= 10 * 8) {
					Game::instance->world->map->changeTile(Vector2(18, 13), PLAYER);
				}
				else {
					Game::instance->world->map->changeTile(Vector2(2, 13), PLAYER);
				}
				generateEnemies(10);
			}
		}

		if (round == 4) { 

			Game::instance->world->map->changeTile(Vector2(19, 4), ARROWRIGHTB);
			Game::instance->world->map->changeTile(Vector2(20, 4), EXIT2);
			
			if (Game::instance->world->map->isObject(Game::instance->world->player->player_pos, EXIT2, EXIT2)) {
				Game::instance->world->map->loadGameMap("data/lab.map");
				Game::instance->world->player->player_pos = Vector2(1 * 8, 4 * 8);
				round = 0;
				counter++;
			}
		}


		//Si tenemos balas hacemos que se muevan hacia donde hayamos disparado y comprobamos si colisionan con los enemigos o con la pared.
		if (bullets[Game::instance->world->player->numBullets] != NULL && Game::instance->world->player->numBullets > 0) {
			bool a = true;
			if (bullets[Game::instance->world->player->numBullets]->shoot) {
				a = bullets[Game::instance->world->player->numBullets]->BulletMove(bullets[Game::instance->world->player->numBullets]->facing);
				for (int i = 0; i < nenemies; i++) {
					if (enemies[i] != NULL) { //Si hay colision con los enemigos, hacemos desaparecer al enemigo y bajamos el contador de enemigos.
						if ((enemies[i]->player_pos.x == bullets[Game::instance->world->player->numBullets]->player_pos.x) && (enemies[i]->player_pos.y == bullets[Game::instance->world->player->numBullets]->player_pos.y)) {
							enemies[i] = NULL;
							Game::instance->synth.playSample("data/roblox.wav", 1, false);
							enemiesinscreen--;
							a = false;
							break;
						}
					}
				}
				if (!a) { //Si ha habido colisión, reducimos el numero de balas.
					if (Game::instance->world->player->numBullets > 0) {
						bullets[Game::instance->world->player->numBullets] = NULL;
						Game::instance->world->player->numBullets--;
					}
				}
			}
		}


		//Si hay enemigos los hacemos moverse.
		if (enemiesinscreen != 0) {
			currenttime += seconds_elapsed;
			if (currenttime >= totalTimer) {
				currenttime -= totalTimer;
				for (int i = 0; i < nenemies; i++) {
					if (enemies[i] == NULL)
						continue;
					if (Game::instance->world->player->player_pos.x > enemies[i]->player_pos.x) {
						if (!enemies[i]->Move(FACE_RIGHT)) {
							if (Game::instance->world->player->player_pos.y > enemies[i]->player_pos.y) {
								enemies[i]->Move(FACE_DOWN);
							}
							else {
								enemies[i]->Move(FACE_UP);
							}
						}
					}
					else if (Game::instance->world->player->player_pos.x < enemies[i]->player_pos.x) {
						if (!enemies[i]->Move(FACE_LEFT)) {
							if (Game::instance->world->player->player_pos.y > enemies[i]->player_pos.y) {
								enemies[i]->Move(FACE_UP);
							}
							else {
								enemies[i]->Move(FACE_DOWN);
							}
						}
					}
					else {
						if (Game::instance->world->player->player_pos.y > enemies[i]->player_pos.y) {
							enemies[i]->Move(FACE_DOWN);
						}
						else {
							enemies[i]->Move(FACE_UP);
						}
					}
				}
			}
		}


		//Comprobamos que los enemigos y el jugador no se toquen, y si se tocan vamos al EndGame.
		for (int i = 0; i < nenemies; i++) {
			if (enemies[i] == NULL) {
				continue;
			}
			if ((enemies[i]->player_pos.x == Game::instance->world->player->player_pos.x) && (enemies[i]->player_pos.y == Game::instance->world->player->player_pos.y)) {
				restart("data/dead.map");
				Game::instance->synth.playSample("data/lose.wav", 1, false);
				changeStage("dead");
			}
		}

		if (round == 2) {
			if (Game::instance->world->map->isObject(Game::instance->world->player->player_pos, PLAYER, GROUNDB)) {
				Game::instance->world->player->numBullets = 5;
			}
		}
		else if (round == 3) {
			if (Game::instance->world->map->isObject(Game::instance->world->player->player_pos, PLAYER, GROUNDB)) {
				Game::instance->world->player->numBullets = 10;
			}
		}
	}
	else if (counter == 3) {
		
		if (round == 0) {
			if (Game::instance->world->map->isObject(Game::instance->world->player->player_pos, EXIT2, EXIT2)) {
				if (Game::instance->world->map->isObject(Vector2(Game::instance->world->player->player_pos.x - 8, Game::instance->world->player->player_pos.y), ARROWRIGHTB, ARROWRIGHTB)) {
					Game::instance->world->map->loadGameMap("data/nothereright.map");
					Game::instance->world->player->player_pos = Vector2(1 * 8, 8 * 8);
					round = 1;
				}
				else if (Game::instance->world->map->isObject(Vector2(Game::instance->world->player->player_pos.x, Game::instance->world->player->player_pos.y + 8), ARROWUPB, ARROWUPB)) {
					Game::instance->world->map->loadGameMap("data/nothereup.map");
					Game::instance->world->player->player_pos = Vector2(10 * 8, 14 * 8);
					round = 2;
				}
				else if (Game::instance->world->map->isObject(Vector2(Game::instance->world->player->player_pos.x, Game::instance->world->player->player_pos.y - 8), ARROWDOWNB, ARROWDOWNB)) {
					Game::instance->world->map->loadGameMap("data/notheredown.map");
					Game::instance->world->player->player_pos = Vector2(6 * 8, 1 * 8);
					round = 3;
				}
				else if (Game::instance->world->map->isObject(Vector2(Game::instance->world->player->player_pos.x + 8, Game::instance->world->player->player_pos.y), ARROWLEFTB, ARROWLEFTB)) {
					counter++;
					Game::instance->world->map->loadGameMap("data/finalboss.map");
					Game::instance->world->player->player_pos = Vector2(19 * 8, 7 * 8);
					counter = 4;
					finalboss = new Sprite();
					finalboss->player_pos = Vector2(1 * 8, 7 * 8);
				}
			}
		}else{
			if (Game::instance->world->map->isObject(Game::instance->world->player->player_pos, EXIT2, EXIT2)) {
				if (Game::instance->world->player->player_pos.x == 10 * 8) {
					Game::instance->world->player->player_pos = Vector2(10 * 8, 1 * 8);
				}
				else if (Game::instance->world->player->player_pos.x == 0) {
					Game::instance->world->player->player_pos = Vector2(19 * 8, 8 * 8);
				}else if (Game::instance->world->player->player_pos.x == 6 * 8) {
					Game::instance->world->player->player_pos = Vector2(6 * 8, 14 * 8);
				}
				Game::instance->world->map->loadGameMap("data/lab.map");
				round = 0;
			}
		}
	}
	else if (counter == 4) {
		totalTimer = 0.3;
		if (Game::instance->world->map->isObject(Game::instance->world->player->player_pos, BULLETB, GROUNDB)) {
			if (Game::instance->world->player->numBullets < 15) {
				Game::instance->world->player->numBullets += 5;
			}
			else {
				Game::instance->world->player->numBullets = 20;
			}
		}

		if (bullets[Game::instance->world->player->numBullets] != NULL && Game::instance->world->player->numBullets > 0) {
			bool a = true;
			if (bullets[Game::instance->world->player->numBullets]->shoot) {
				a = bullets[Game::instance->world->player->numBullets]->BulletMove(bullets[Game::instance->world->player->numBullets]->facing);
				if (finalboss != NULL) { //Si hay colision con los enemigos, hacemos desaparecer al enemigo y bajamos el contador de enemigos.
					if ((finalboss->player_pos.x == bullets[Game::instance->world->player->numBullets]->player_pos.x) && (finalboss->player_pos.y == bullets[Game::instance->world->player->numBullets]->player_pos.y)) {
						finalboss->life --;
						Game::instance->synth.playSample("data/roblox.wav", 1, false);
						enemiesinscreen--;
						a = false;
						
					}
				}
			
				if (!a) { //Si ha habido colisión, reducimos el numero de balas.
					if (Game::instance->world->player->numBullets > 0) {
						bullets[Game::instance->world->player->numBullets] = NULL;
						Game::instance->world->player->numBullets--;
					}
				}
			}
		}

		if (Game::instance->world->player->player_pos == finalboss->player_pos) {
			restart("data/dead.map");
			Game::instance->synth.playSample("data/lose.wav", 1, false);
			changeStage("dead");
		}

		currenttime += seconds_elapsed;
		if (currenttime >= totalTimer) {
			currenttime -= totalTimer;
			if (finalboss != NULL) {
				if (Game::instance->world->player->player_pos.x > finalboss->player_pos.x) {
					if (!finalboss->Move(FACE_RIGHT)) {
						if (Game::instance->world->player->player_pos.y > finalboss->player_pos.y) {
							finalboss->Move(FACE_DOWN);
						}
						else {
							finalboss->Move(FACE_UP);
						}
					}
				}
				else if (Game::instance->world->player->player_pos.x < finalboss->player_pos.x) {
					if (!finalboss->Move(FACE_LEFT)) {
						if (Game::instance->world->player->player_pos.y > finalboss->player_pos.y) {
							finalboss->Move(FACE_UP);
						}
						else {
							finalboss->Move(FACE_DOWN);
						}
					}
				}
				else {
					if (Game::instance->world->player->player_pos.y > finalboss->player_pos.y) {
						finalboss->Move(FACE_DOWN);
					}
					else {
						finalboss->Move(FACE_UP);
					}
				}
			}
				
		}


		if (finalboss->life == 0) {
			restart("data/win.map");
			Game::instance->synth.playSample("data/win.wav", 1, false);
			changeStage("win");
		}

	}
}

//Funcion para resetear las variables cambiadas durante el juego.
void PlayStage::restart(const char* map) {
	Game::instance->world->map->loadGameMap(map);
	Game::instance->world->player->shoot = false;
	Game::instance->world->player->numBullets = 0;
	Game::instance->world->player->player_pos = Vector2(2 * 8, 8 * 8);
	nenemies = 0;
	enemiesinscreen = 0;
	counter = 0;
	round = 0;
}

//Funcion que genera enemigos de forma aleatoria (la seed se genera en el main.cpp) y generamos una posicion aleatoria.
//Comprobamos que es correcta y que esta a X distancia de nuestro jugador y entre si.
void PlayStage::generateEnemies(int num) {

	for (int x = 0; x < num; x++) {

		int randnumi = rand() % (19) + 1;
		int randnumj = rand() % (14) + 1;

		if (Game::instance->world->map->isValid(Vector2(randnumi*8, randnumj * 8),FACE_RIGHT) && Game::instance->world->map->isValid(Vector2(randnumi * 8, randnumj * 8), FACE_LEFT)
			&&
			Game::instance->world->map->isValid(Vector2(randnumi * 8, randnumj * 8), FACE_UP) && Game::instance->world->map->isValid(Vector2(randnumi * 8, randnumj * 8), FACE_DOWN)) {
			if (x > 0) {
				for (int y = 0; y < x; y++) {
					if ((randnumi - floor(Game::instance->world->player->player_pos.x / 8) > 2 || randnumi - floor(Game::instance->world->player->player_pos.x) < -2)
						&&
						(randnumj - floor(Game::instance->world->player->player_pos.y/8) > 2 || randnumj - floor(Game::instance->world->player->player_pos.y/8) < -2)) {
						
						enemies[x] = new Sprite(Vector2(randnumi * 8, randnumj * 8), FACE_LEFT, 10);
						nenemies = x + 1;
						enemiesinscreen = nenemies;
					}
					else {
						x--;
						break;
					}
				}
			}
			else {
				if ((randnumi - floor(Game::instance->world->player->player_pos.x/8) > 2 || randnumi - floor(Game::instance->world->player->player_pos.x/8) < -2)
					&& 
					(randnumj - floor(Game::instance->world->player->player_pos.y/8) > 2 || randnumj - floor(Game::instance->world->player->player_pos.y/8) < -2)) {
					enemies[x] = new Sprite(Vector2(randnumi * 8, randnumj * 8), FACE_LEFT, 10); nenemies = x + 1; enemiesinscreen = nenemies; }
				else {
					x--;
				}
			}
		}
		else {
			x--;
		}
	}
}

void MenuStage::render(Image& fb) {
	//Menu del juego, 
	Game::instance->world->map->render(fb, Game::instance->world->tileset);

	fb.drawText("Play", fb.width / 2 - 5, fb.height / 2 - 10, Game::instance->world->minifontw, 4, 6);
	fb.drawText("Tutorial", fb.width / 2 - 13, fb.height / 2, Game::instance->world->minifontw, 4, 6);
	fb.drawText("Exit", fb.width / 2-5, fb.height / 2 + 10, Game::instance->world->minifontw, 4, 6);

	if (option == 0) {
		fb.drawLine(fb.width / 2-5, fb.height / 2 - 3, fb.width / 2 + 10, fb.height / 2 - 3, Color::WHITE);
	}
	if (option == 1) {
		fb.drawLine(fb.width / 2 - 13, fb.height / 2 + 7, fb.width / 2 + 18, fb.height / 2 + 7, Color::WHITE);
	}
	if (option == 2) {
		fb.drawLine(fb.width / 2 -5, fb.height / 2 + 17, fb.width / 2 + 10, fb.height / 2 + 17, Color::WHITE);
	}
}

void MenuStage::update(double seconds_elapsed) {
	//Dependiendo de la opcion elegida iremos a un Stage u otro.
	if (Input::wasKeyPressed(SDL_SCANCODE_DOWN)) {
		if (option == 2)
			option = 0;
		else
			option++;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_UP)) {
		if (option == 0)
			option = 2;
		else
			option--;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_A)) {
		switch (option) {
			case 0:	Game::instance->world->map->loadGameMap("data/intro.map");changeStage("play"); break;
			case 1: Game::instance->world->map->loadGameMap("data/tutorial.map");changeStage("tutorial"); break;
			case 2: Game::instance->must_exit = true; break;
		}
	}
}

void TutorialStage::render(Image& fb) {

	fb.fill(Color::BLACK);

	if (counter == 0) {
		fb.drawText("Use arrow keys to move", fb.width / 2 - 45, fb.height / 2 - 10, Game::instance->world->minifontw, 4, 6);
		fb.drawText("Each time you press a key", fb.width / 2 - 48, fb.height / 2, Game::instance->world->minifontw, 4, 6);
		fb.drawText("you'll move 1 position", fb.width / 2 - 45, fb.height / 2 + 10, Game::instance->world->minifontw, 4, 6);
		fb.drawText(" Press A to continue", fb.width / 2 - 20, fb.height / 2 + 30, Game::instance->world->minifontw, 4, 6);
	}
	else {
		fb.drawText("Collect Gun and Bullet to shoot", fb.width / 2 - 60, fb.height / 2 - 30, Game::instance->world->minifontw, 4, 6);
		fb.drawText("Press SPACE to kill unicorns", fb.width / 2 - 55, fb.height / 2 - 20, Game::instance->world->minifontw, 4, 6);
		fb.drawText("At the end of each round, find your", fb.width / 2 - 70, fb.height / 2 - 10, Game::instance->world->minifontw, 4, 6);
		fb.drawText("friend and join him to receive", fb.width / 2 - 60, fb.height / 2, Game::instance->world->minifontw, 4, 6);
		fb.drawText("more bullets", fb.width / 2 - 30, fb.height / 2 + 10, Game::instance->world->minifontw, 4, 6);
		fb.drawText(" Press A to continue", fb.width / 2 - 20, fb.height / 2 + 30, Game::instance->world->minifontw, 4, 6);
	}
}

void TutorialStage::update(double seconds_elapsed) {

	if (Input::wasKeyPressed(SDL_SCANCODE_A)) //if key left
	{
		if (counter == 0) {
			counter++;
		}
		else {
			counter = 0;
			Game::instance->world->map->loadGameMap("data/menu.map");
			changeStage("menu");
		}
	}
}

void StartStage::render(Image& fb) {

	Game::instance->world->map->render(fb, Game::instance->world->tileset);


	fb.drawText(" Press A to start the game", fb.width / 2 - 55, fb.height / 2 + 50, Game::instance->world->minifontw, 4, 6);

}

void StartStage::update(double seconds_elapsed) {

	if (Input::wasKeyPressed(SDL_SCANCODE_A)) //if key left
	{
		Game::instance->world->map->loadGameMap("data/menu.map");
		changeStage("menu");
	}
}

void DeadStage::update(double seconds_elapsed) {

	if (Input::wasKeyPressed(SDL_SCANCODE_A)) //if key left
	{
		Game::instance->world->map->loadGameMap("data/menu.map");
		changeStage("menu");
	}
}


void DeadStage::render(Image& fb) {

	Game::instance->world->map->render(fb, Game::instance->world->tileset);


	fb.drawText(" Press A to continue", 1*8, 0, Game::instance->world->minifontw, 4, 6);

}

void WinStage::update(double seconds_elapsed) {

	if (Input::wasKeyPressed(SDL_SCANCODE_A)) //if key left
	{
		Game::instance->world->map->loadGameMap("data/menu.map");
		changeStage("menu");
	}
}


void WinStage::render(Image& fb) {

	Game::instance->world->map->render(fb, Game::instance->world->tileset);
	fb.drawText(" Press A to continue", 1 * 8, 0, Game::instance->world->minifontw, 4, 6);

}


