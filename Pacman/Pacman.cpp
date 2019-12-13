#include "Pacman.h"
#include <iostream>
#include <time.h>
#include <sstream>
#include <fstream>
#include<cstdlib>
#include <chrono>


Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250)
{
	//Ini member variables
	for (int i = 0; i < PLAYERCOUNT; i++)
	{
		_pacman[i] = new Player();
		_pacman[i]->infected = false;
		_pacman[i]->speedMultiplier = 1.0f;
		_pacman[i]->infectedValue = 0;
		_pacman[i]->_speedPowerup = 0;
		_pacman[i]->powerupTimer = 0;
		_pacman[i]->_PowerupPacmanSpeed = 0;
		_pacman[i]->_aiRandomCounter = 60;
	}
	_paused = false;
	_pKeyDown = false;
	_start = true;
	infectedCount = 0;
	chosenInfected = false;
	timer = std::chrono::high_resolution_clock::now();
	_infectedWin = false;
	_survivorWin = false;
	_pop = new SoundEffect();
	_startSound = new SoundEffect();
	_powerup = new SoundEffect();
	_infect = new SoundEffect();
	_start = new SoundEffect();
	_gameEnd = new SoundEffect();
	_munchiesRemaining = MUNCHIECOUNT;
	_gameOverTimer = 0;


	srand(time(NULL));

	//Local Variable
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new Enemy();
		_munchies[i]->frame = rand() % 1;
		_munchies[i]->currentFrameTime = 0;
		_munchies[i]->frameTime = rand() % 500 + 50;
	}
	for (int i = 0; i < CHERRYCOUNT; i++)
	{
		_cherry[i] = new Enemy();
		_cherry[i]->frame = rand() % 1;
		_cherry[i]->currentFrameTime = 0;
		_cherry[i]->frameTime = rand() % 500 + 50;
	}
	Audio::Initialise();

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	//Clean up pointers
	for (int i = 0; i < PLAYERCOUNT; i++)
	{
		delete _pacman[i]->texture;
		delete _pacman[i]->sourceRect;
		delete _pacman[i]->position;
	}
	delete _munchieTexture;
	delete _munchieRect;
	delete _pop;
}
//Check Paused Function
void Pacman::checkPaused(Input::KeyboardState* keyboardState, Input::Keys pausekey)
{
	//Pause
	if (keyboardState->IsKeyDown(Input::Keys::P) && !_pKeyDown && !_start)
	{
		_pKeyDown = true;
		_paused = !_paused;
	}
	if (keyboardState->IsKeyUp(Input::Keys::P) && !_start)
	{
		_pKeyDown = false;
	}
}
//Check Started
void Pacman::checkStarted(Input::KeyboardState* keyboardState, Input::Keys startkey)
{
	//Check if start
	if (_start)
	{
		//Start game
		if (keyboardState->IsKeyDown(Input::Keys::SPACE) && _start)
		{
			_start = false;
		}
	}
}

//Input Function
void Pacman::Input(int elapsedTime, Input::KeyboardState* keyboardState, Input::MouseState* mouseState)
{
		float pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacman[0]->speedMultiplier;
	// Checks if D key is pressed
	if (keyboardState->IsKeyDown(Input::Keys::D))
	{
		if (_pacman[0]->_speedPowerup == true)
		{
			//Moves Pacman Right
			_pacman[0]->position->X += pacmanSpeed += _pacman[0]->_PowerupPacmanSpeed;
			_pacman[0]->direction = 0;
		}
		else
		{
			//Moves Pacman Right
			_pacman[0]->position->X += pacmanSpeed;
			_pacman[0]->direction = 0;
		}
	}
	// Checks if A key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::A))
	{
		if (_pacman[0]->_speedPowerup == true)
		{
			//Moves Pacman Left
			_pacman[0]->position->X -= pacmanSpeed += _pacman[0]->_PowerupPacmanSpeed;
			_pacman[0]->direction = 2;
		}
		else
		{
			//Moves Pacman Left
			_pacman[0]->position->X -= pacmanSpeed;
			_pacman[0]->direction = 2;
		}
	}
	// Checks if W key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::W))
	{
		if (_pacman[0]->_speedPowerup == true)
		{
			//Moves Pacman Up
			_pacman[0]->position->Y -= pacmanSpeed += _pacman[0]->_PowerupPacmanSpeed; //Moves Pacman Up, uses minus position since OpenGL counts grid from top left
			_pacman[0]->direction = 3;
		}
		else
		{
			//Moves Pacman Up
			_pacman[0]->position->Y -= pacmanSpeed; //Moves Pacman Up, uses minus position since OpenGL counts grid from top left
			_pacman[0]->direction = 3;
		}
	}
	// Checks if S key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::S))
	{
		if (_pacman[0]->_speedPowerup == true)
		{
			//Moves Pacman Down
			_pacman[0]->position->Y += pacmanSpeed += _pacman[0]->_PowerupPacmanSpeed; //Moves Pacman Down
			_pacman[0]->direction = 1;
		}
		else
		{
			//Moves Pacman Down
			_pacman[0]->position->Y += pacmanSpeed; //Moves Pacman Down
			_pacman[0]->direction = 1;
		}
	}

	/*//Speed Multiplier
	if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT))
	{
		//Apply Multiplier
		_pacman[0]->speedMultiplier = 2.0f;
	}
	else
	{
		//Reset Multiplier
		_pacman[0]->speedMultiplier = 1.0f;
	}

	//Randomises cherry position on R press
	if (keyboardState->IsKeyDown(Input::Keys::R))
	{
		for (int i = 0; i < CHERRYCOUNT; i++)
		{
			_cherry[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		}
	}*/

	//Mouse Input
	mouseState->LeftButton;
	//Move cherry to mouse on left click
	if (mouseState->LeftButton == Input::ButtonState::PRESSED)
	{
		for (int i = 0; i < CHERRYCOUNT; i++)
		{
			_cherry[i]->position->X = mouseState->X;
			_cherry[i]->position->Y = mouseState->Y;
		}
	}
}

//Infected Choosing
void Pacman::chooseFirstIntected(int elapsedTime)
{
	
	int seconds;
	int fps = 60;

	if (!_start && !_paused)
	{
		if (elapsedTime > 8)
		{
			for (int i = 0; i < PLAYERCOUNT; i++)
			{
				//Timer
				_pacman[i]->timeFrameTime++;
				if (_pacman[i]->timeFrameTime == 63)
				{
					_pacman[i]->Time++;
					_pacman[i]->timeFrameTime = 0;
					cout << _pacman[0]->Time << endl;
				}

				//Infect first Pacman
				if (_pacman[i]->Time == 10 && chosenInfected == false)
				{
					int infectedRand;
					infectedRand = rand() % 3;
					cout << "Pacman " << infectedRand << " has been infected." << endl;
					_pacman[infectedRand]->infected = true;
					_pacman[infectedRand]->infectedValue = 64;
					chosenInfected = true;
					infectedCount++;
					Audio::Play(_infect);
				}
			}
		}
	}	
}

//Viewport Collision for PLAYER AND AI
void Pacman::checkViewportCollision()
{
	for (int i = 0; i < PLAYERCOUNT; i++)
	{
		//Check Viewport Collision
		if (_pacman[i]->position->X + _pacman[i]->sourceRect->Width > Graphics::GetViewportWidth())
		{
			// Pacman Hit right wall - reset his position
			_pacman[i]->position->X = Graphics::GetViewportWidth() - 1023;
		}
		if (_pacman[i]->position->X < 0)
		{
			// Pacman Hit left wall - reset his position
			_pacman[i]->position->X = Graphics::GetViewportWidth() - 1 - _pacman[i]->sourceRect->Width;
		}
		if (_pacman[i]->position->Y + _pacman[i]->sourceRect->Height > Graphics::GetViewportHeight())
		{
			// Pacman Hit bottom wall - reset his position
			_pacman[i]->position->Y = 0;
		}
		if (_pacman[i]->position->Y < 0)
		{
			// Pacman Hit top wall - reset his position
			_pacman[i]->position->Y = Graphics::GetViewportHeight() - 1 - _pacman[i]->sourceRect->Height;
		}
	}
}
//Update Pacman
void Pacman::updatePacman(Player* PlayerStruct, int elapsedTime)
{
	for (int i = 0; i < PLAYERCOUNT; i++)
	{
		//Update Pacman Frame
		if (!_paused && !_start)
		{
		}
		_pacman[i]->currentFrameTime += elapsedTime;

		if (_pacman[i]->currentFrameTime > _cPacmanFrameTime)
		{
			_pacman[i]->frame++;

			if (_pacman[i]->frame >= 2)
				_pacman[i]->frame = 0;

			_pacman[i]->currentFrameTime = 0;
		}


		_pacman[i]->sourceRect->X = _pacman[i]->sourceRect->Width * _pacman[i]->frame + _pacman[i]->infectedValue;
		_pacman[i]->sourceRect->Y = _pacman[i]->sourceRect->Height * _pacman[i]->direction;
	}
}
//AI Movement
void Pacman::updateAI(Player* ai, int elapsedTime)
{
	//Random Direction
	if (ai->_aiRandomCounter == 0)
	{
		ai->direction = rand() % 4;
		ai->_aiRandomCounter = 60;
	}
	else
		ai->_aiRandomCounter--;

	float pacmanSpeed = _cPacmanSpeed * elapsedTime;
	//Right
	if (ai->direction == 0)
	{
		if (ai->_speedPowerup == true)
			ai->position->X += pacmanSpeed += ai->_PowerupPacmanSpeed;
		else
			ai->position->X += pacmanSpeed;
	}
	//Down
	else if (ai->direction == 1)
	{
		if(ai->_speedPowerup == true)
			ai->position->Y += pacmanSpeed += ai->_PowerupPacmanSpeed;
		else
			ai->position->Y += pacmanSpeed;
	}
	//Left
	else if (ai->direction == 2)
	{
		if (ai->_speedPowerup == true)
			ai->position->X -= pacmanSpeed += ai->_PowerupPacmanSpeed;
		else
			ai->position->X -= pacmanSpeed;
	}
	//Up
	else if (ai->direction == 3)
	{
		if (ai->_speedPowerup == true)
			ai->position->Y -= pacmanSpeed += ai->_PowerupPacmanSpeed;
		else
			ai->position->Y -= pacmanSpeed;
	}
}

//Collision
void Pacman::checkCollisions()
{
	for (int i = 0; i < PLAYERCOUNT; i++)
	{
		
		int bottom1 = _pacman[i]->position->Y + _pacman[i]->sourceRect->Height;
		int bottom2 = 0;
		int left1 = _pacman[i]->position->X;
		int left2 = 0;
		int right1 = _pacman[i]->position->X + _pacman[i]->sourceRect->Width;
		int right2 = 0;
		int top1 = _pacman[i]->position->Y;
		int top2 = 0;

		//Munchie Collision
		if (!_pacman[i]->infected)
		{
			for (int j = 0; j < MUNCHIECOUNT; j++)
			{
				bottom2 = _munchies[j]->position->Y + _munchies[j]->sourceRect->Height;
				left2 = _munchies[j]->position->X;
				right2 = _munchies[j]->position->X + _munchies[j]->sourceRect->Width;
				top2 = _munchies[j]->position->Y;

				if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
				{
					_munchies[j]->position->X = -200;
					_munchies[j]->position->X = -200;
					_munchiesRemaining--;
					Audio::Play(_pop);
				}
				if (_munchiesRemaining == 0)
				{

				}
			}
		}
		//Cherry Collision & Powerup Code
		for (int j = 0; j < CHERRYCOUNT; j++)
		{
			bottom2 = _cherry[j]->position->Y + _cherry[j]->sourceRect->Height;
			left2 = _cherry[j]->position->X;
			right2 = _cherry[j]->position->X + _cherry[j]->sourceRect->Width;
			top2 = _cherry[j]->position->Y;

			if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
			{
				_cherry[j]->position->X = -200;
				_cherry[j]->position->X = -200;
				Audio::Play(_powerup);
				_pacman[i]->_speedPowerup = true;
				_pacman[i]->_PowerupPacmanSpeed = 4;
			}
			if (_pacman[i]->_speedPowerup == true)
			{
				_pacman[i]->powerupTimer++;
			}
			if (_pacman[i]->_speedPowerup == true && _pacman[i]->powerupTimer == 1200)
			{
				_pacman[i]->_speedPowerup = false;
				_pacman[i]->_PowerupPacmanSpeed = 0;
			}
		}
		//Infected Collision
		if (_pacman[i]->infected)
		{
			for (int j = 0; j < PLAYERCOUNT; j++)
			{
				if (i != j)
				{
					bottom2 = _pacman[j]->position->Y + _pacman[j]->sourceRect->Height;
					left2 = _pacman[j]->position->X;
					right2 = _pacman[j]->position->X + _pacman[j]->sourceRect->Width;
					top2 = _pacman[j]->position->Y;
					//if ((_pacman[i]->infected == true.bottom1 > _pacman[i]->infected == true.top2))
					if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2) && _pacman[j]->infected == false)
					{
						_pacman[j]->infected = true;
						_pacman[j]->infectedValue = 64;
						cout << "Pacman " << j << " is now infected" << endl;
						infectedCount++;
						Audio::Play(_infect);
					}
				}
			}
		}
	}
}
//Update Munchie
void Pacman::updateMunchie(Enemy* Munchie, int elapsedTime)
{
	//Update Munchie Frame Count
	Munchie->currentFrameTime += elapsedTime;

	if (Munchie->currentFrameTime > Munchie->frameTime)
	{
		Munchie->frame++;

		if (Munchie->frame >= 2)
			Munchie->frame = 0;

		Munchie->currentFrameTime = 0;
	}
	Munchie->sourceRect->X = Munchie->sourceRect->Width * Munchie->frame;
}
void Pacman::updateCherry(Enemy* cherry, int elapsedTime)
{
	//Update Cherry Frame Count
	cherry->currentFrameTime += elapsedTime;

	if (cherry->currentFrameTime > cherry->frameTime)
	{
		cherry->frame++;

		if (cherry->frame >= 2)
			cherry->frame = 0;

		cherry->currentFrameTime = 0;
	}
	cherry->sourceRect->X = cherry->sourceRect->Width * cherry->frame;
}
void Pacman::winConditions(Input::KeyboardState* keyboardState, Input::Keys startkey, int elapsedTime)
{
	if (infectedCount >= 4)
	{
		_infectedWin = true;
	}
	if (_munchiesRemaining <= 0)
	{
		_survivorWin = true;
	}
	Audio::Play(_gameEnd);
}

//Load Content Function
void Pacman::LoadContent()
{
	// Local Variable
	Texture2D* munchieTex = new Texture2D();
	Texture2D* cherryTex = new Texture2D();

	//Set Menu Parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	// Load Pacman & Infected Pacman
	for (int i = 0; i < PLAYERCOUNT; i++)
	{
		_pacman[i]->texture = new Texture2D();
		_pacman[i]->position = new Vector2(32.0f * i, 32.0f * i);
		_pacman[i]->texture->Load("Textures/Pacman.png", false);
		_pacman[i]->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	}
	
	// Load Munchie
	munchieTex->Load("Textures/Munchie.png", true);

	//Load Pop
	_pop->Load("Sounds/Munchie.wav");
	_startSound->Load("Sounds/Start.wav");
	_infect->Load("Sounds/Infection.wav");
	_powerup->Load("Sounds/Powerup.wav");
	_gameEnd->Load("Sounds/GameEnd.wav");


	
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->texture = munchieTex;	
		_munchies[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_munchies[i]->sourceRect = new Rect(0.0f, 0.0f, 12, 12);
	}
	//Load Cherry
	cherryTex->Load("Textures/Cherry.png", true);

	for (int i = 0; i < CHERRYCOUNT; i++)
	{
		_cherry[i]->texture = cherryTex;
		_cherry[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_cherry[i]->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	}

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();
	//Check for Pause & game start
	checkPaused(keyboardState, Input::Keys::P);
	checkStarted(keyboardState, Input::Keys::SPACE);


	//If not paused & game has started, call functions and update
	if (!_paused && !_start && !_infectedWin && !_survivorWin)
	{
		//Player Movement
		Input(elapsedTime, keyboardState, mouseState);
		chooseFirstIntected(elapsedTime);
		//AI Movement, i set to 1 due to 0 being player
		for (int i = 1; i < PLAYERCOUNT; i++)
		{
			updateAI(_pacman[i], elapsedTime);
			checkCollisions();
		}
		for (int i = 0; i < PLAYERCOUNT; i++)
		{
			updatePacman(_pacman[i], elapsedTime);
		}
		for (int i = 0; i < MUNCHIECOUNT; i++)
		{
			updateMunchie(_munchies[i], elapsedTime);

		}
		for (int i = 0; i < CHERRYCOUNT; i++)
		{
			updateCherry(_cherry[i], elapsedTime);

		}
		checkViewportCollision();
	}
	winConditions(keyboardState, Input::Keys::SPACE, elapsedTime);
}

void Pacman::Draw(int elapsedTime)
{
	// Starts Drawing
	SpriteBatch::BeginDraw();

	// Allows us to easily create a string
	std::stringstream stream;


	stream << "Timer: " << _pacman[0]->Time << "\n" << "Munchies Remaining: " << _munchiesRemaining << endl;;

	for (int i = 0; i < PLAYERCOUNT; i++)
	{
		if (_pacman[i]->infected == true)
			stream << "Pacman " << i + 1 << ": Infected. \n";
		else
			stream << "Pacman " << i + 1 << ": Not infected. \n";

		//stream << "Pacman X: " << _pacman[0]->position->X << "\n" << " Y: " << _pacman[0]->position->Y;
	}
	for (int i = 0; i < PLAYERCOUNT; i++)
	{
		// Draws Pacman
		SpriteBatch::Draw(_pacman[i]->texture, _pacman[i]->position, _pacman[i]->sourceRect);
	}
	// Draws Munchie
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		SpriteBatch:: Draw(_munchies[i]->texture, _munchies[i]->position, _munchies[i]->sourceRect);
	}
	// Draws Cherry
	for (int i = 0; i < CHERRYCOUNT; i++)
	{
		SpriteBatch::Draw(_cherry[i]->texture, _cherry[i]->position, _cherry[i]->sourceRect);
	}

	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	if (_paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}
	if (_start)
	{
		std::stringstream menuStream;
		menuStream << "Press Space to Play!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}
	if (_infectedWin)
	{
		std::stringstream menuStream;
		menuStream << "Infected Win!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}
	if (_survivorWin)
	{
		std::stringstream menuStream;
		menuStream << "Survivors Win!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}
	SpriteBatch::EndDraw(); // Ends Drawing
}