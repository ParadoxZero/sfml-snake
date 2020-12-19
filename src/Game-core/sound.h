#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

// In order for this script to work you will need to add additional dependencies 
// properties --> linker --> input --> Additional dependencies --> sfml-audio-d.lib
// whenever setting up project on different system remember to change sound paths
#define eatSoundPath "C:/Users/HP/source/repos/sfml-snake/sounds/bite.wav"
#define BGMPath "C:/Users/HP/source/repos/sfml-snake/sounds/BGM.wav"
#define deathSoundPath "C:/Users/HP/source/repos/sfml-snake/sounds/death.wav"
#define menuMusicPath "C:/Users/HP/source/repos/sfml-snake/sounds/mainMenu.wav"
#define MAX_NUMBER_OF_ITEMS 3



class sound
{
public:
	sound();
	void onCollisionSound();
	void BGM();
	void menuMusic(bool x);
	bool isDeath;
	bool isFood;

private:
	sf::SoundBuffer eatSoundBuffer;
	sf::SoundBuffer deathSoundBuffer;
	sf::Music bgm;
	sf::Music MM;
	sf::Sound eatSound;
	sf::Sound deathSound;
};

