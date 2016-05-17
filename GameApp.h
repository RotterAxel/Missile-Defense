//Programmer: Axel Rotter Morgado
//Date: 18.04.2016


#ifndef GAMEAPP_H
#define GAMEAPP_H

//Include directories from existing libraries
#include <tchar.h>
#include <string>
#include <Windows.h>
#include <list>
#include <cstdlib>
#include <ctime>

//Includes from the header files of this project
#include "Sprite.h"
#include "Circle.h"
#include "..\\2DEngine\resource.h"
#include "CTimer.h"
#include "BackBuffer.h"
#include "Rect.h"
#include "Detonation.h"
#include "PlayerMissile.h"
#include "EnemyMissile.h"
#include "SpawnTimer.h"

class GameApp
{
public:	
	//Standard constructor and destructor
	GameApp();
	~GameApp();

	//Public methods for the main application to access
	LRESULT DisplayWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	bool InitInstance(HINSTANCE hInstance, LPCTSTR lpCmdLine, int iCmdShow);
	int BeginGame();
	bool ShutDown();

private:
	//---------------------------------------------------------------------------
	//Base template methods available from the engine for 2D game creation.
	//---------------------------------------------------------------------------
	bool CreateDisplay();
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	void FrameAdvance();
	void Draw();
	void Update();
	void Pause();
	void Unpause();
	Vec2 ScreenToClientCoords();
	void InitializeSprites();
	void ResetGameState();

	//---------------------------------------------------------------------------
	//Game methods that add mechanics and specifics to a 2D game project
	//---------------------------------------------------------------------------
	void UpdateReticle();
	void UpdateMissiles();
	void UpdateEnemyMissiles();
	void UpdateDetonation();
	void Detonate(Vec2 A);
	void CreateEnemyMissile();
	void DrawPlayerScore();
	void DrawPlayerLives();

	//A method to avoid clutter in the main code body.
	void LoadScoreSprites(HINSTANCE hInstance);

	//Handles
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	HDC			m_hSpriteDC;

	//Constant variables to determine the client window size
	const int	m_cClientWidth = 864;
	const int	m_cClientHeight = 520;
	const int	m_cWindowWidth = 870;
	const int	m_cWindowHeight = 556;
	const POINT m_cClientCenter = {	m_cClientWidth / 2, 
									m_cClientHeight / 2 };
	
	//Variable data members
	int				m_PlayerLives;
	int				m_PlayerScore;
	bool			m_Paused;
	bool			m_GameOver;
	std::string		m_WndCaption;	

	//Game objects
	CTimer			m_oTimer;
	BackBuffer*		m_oBackBuffer;

	
	Sprite* mGameBoard;
	Sprite* mSilo;
	Sprite* mReticle;
	Sprite* mPlayerMissileSprite;
	Sprite* mDetonationArray[10];
	Sprite* mEnemyMissileSprite;
	Sprite*	mPauseScreen;
	Sprite* mGameOverSprite;

	PlayerMissile*	mPlayerMissile;
	Detonation*		mDetonation;
	EnemyMissile*	mEnemyMissile;
	SpawnTimer*		mEnemySpawnTimer;

	POINT	mLastMousePos;
	POINT	mCurrMousePos;

	Rect	mBoardBounds;

	//User Interface
	Sprite* mScore;
	Sprite* mLives;
	Sprite* mHeart;
	Sprite* mPlayerOnes[10];
	Sprite* mPlayerDecimals[10];
	Sprite* mPlayerHundreds[10];

	//Lists that store coordinates
	std::list<Detonation>		gMissileDet;
	std::list<PlayerMissile>	gPlayerMissiles;
	std::list<EnemyMissile>		gEnemyMissiles;
};

#endif //GAMEAPP_H