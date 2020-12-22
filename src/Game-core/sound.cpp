#include "sound.h"
#include <iostream>



SoundController::SoundController()
{
	if (!eatSoundBuffer.loadFromFile(eatSoundPath))
	{
		std::cout << "hit sound file is either corrupted or location given is wrong" << std::endl;
	}

	if (!deathSoundBuffer.loadFromFile(deathSoundPath))
	{
		std::cout << "death sound file is either corrupted or location given is wrong" << std::endl;
	}
	
	if (!bgm.openFromFile(BGMPath))
	{
		std::cout << "BGM sound file is either corrupted or location given is wrong" << std::endl;
	}

	if (!MM.openFromFile(menuMusicPath))
	{
		std::cout << "main menu sound file is either corrupted or location given is wrong" << std::endl;
	}

	eatSound.setBuffer(eatSoundBuffer);	
	deathSound.setBuffer(deathSoundBuffer);
	bgm.setVolume(60);
	MM.setVolume(60);
}

void SoundController::playDeathSound()
{
	deathSound.play();
	while (deathSound.getStatus() == sf::Sound::Playing)
	{
		sf::sleep(sf::milliseconds(100));
	}
}

void SoundController::playFoodSound()
{
	eatSound.play();
}

void SoundController::BGM()
{
	if(bgm.getStatus() == sf::Music::Playing)
	{
		bgm.stop();
		return;
	}
	bgm.play();
	bgm.setLoop(true);
}

void SoundController::playMenuMusic()
{
	MM.play();
	MM.setLoop(true);
}

void SoundController::stopMenuMusic()
{
	MM.stop();
}