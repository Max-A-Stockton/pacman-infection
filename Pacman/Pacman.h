#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

#define PLAYERCOUNT 4
#define MUNCHIECOUNT 50
#define CHERRYCOUNT 4
// Just need to include main header file
#include "S2D/S2D.h"
#include <chrono>

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

//Structure Definition
struct Player
{
	bool infected;
	float speedMultiplier;
	int currentFrameTime;
	int direction;
	int frame;
	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
	int Time;
	int timeFrameTime;
	int infectedValue;
	bool _speedPowerup;
	int powerupTimer;
	int _PowerupPacmanSpeed;
	int _aiRandomCounter;
};
struct Enemy
{
	int currentFrameTime;
	int frame;
	int frameTime;
	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
};

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	Player* _pacman[PLAYERCOUNT];
	Enemy* _munchies[MUNCHIECOUNT];
	Enemy* _cherry[CHERRYCOUNT];
	//Data for Menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	bool _paused;
	bool _pKeyDown;
	bool _start;
	bool _infectedWin;
	bool _survivorWin;
	const int _cPacmanFrameTime;
	int infectedCount;
	bool chosenInfected;
	std::chrono::high_resolution_clock::time_point timer;
	SoundEffect* _pop;
	SoundEffect* _startSound;
	SoundEffect* _infect;
	SoundEffect* _powerup;
	SoundEffect* _gameEnd;
	int _munchiesRemaining;
	int _gameOverTimer;



	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState*mouseState);
	void checkPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void checkStarted(Input::KeyboardState* state, Input::Keys startkey);
	void checkViewportCollision();
	void updatePacman(Player* playerStruct, int elapsedTime);
	void updateMunchie(Enemy* munchieStruct, int elapsedTime);
	void updateCherry(Enemy* cherryStruct, int elapsedTime);
	void checkCollisions();
	void updateAI(Player* ai, int elapsedTime);
	void chooseFirstIntected(int elapsedTime);
	void winConditions(Input::KeyboardState* keyboardState, Input::Keys startkey, int elapsedTime);

	// Data to represent Pacman
	Vector2* _pacmanPosition;
	Rect* _pacmanSourceRect;
	Texture2D* _pacmanTexture;

	//Data to represent Munchie
	Vector2* _munchiePosition;

	//Constant data for Game Variables
	const float _cPacmanSpeed;

	// Data to represent Munchie
	int _munchieframeCount;
	Rect* _munchieRect;
	Texture2D* _munchieTexture;

	// Position for String
	Vector2* _stringPosition;

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};