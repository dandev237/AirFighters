#include "Player.h"
#include "CommandQueue.h"
#include "Aircraft.h"

struct AircraftMover
{
	AircraftMover(float vx, float vy)
		: velocity(vx, vy)
	{
	}

	void operator() (Aircraft& aircraft, sf::Time) const
	{
		aircraft.accelerate(velocity);
	}

	sf::Vector2f velocity;
};

Player::Player()
{
	mKeyBinding[sf::Keyboard::Up] = Action::MoveUp;
	mKeyBinding[sf::Keyboard::Down] = Action::MoveDown;
	mKeyBinding[sf::Keyboard::Left] = Action::MoveLeft;
	mKeyBinding[sf::Keyboard::Right] = Action::MoveRight;

	const float playerSpeed = 200.f;

	//Initial action bindings
	mActionBinding[Action::MoveUp].action = derivedAction<Aircraft>(AircraftMover(0.f, -playerSpeed));
	mActionBinding[Action::MoveDown].action = derivedAction<Aircraft>(AircraftMover(0.f, +playerSpeed));
	mActionBinding[Action::MoveLeft].action = derivedAction<Aircraft>(AircraftMover(-playerSpeed, 0.f));
	mActionBinding[Action::MoveRight].action = derivedAction<Aircraft>(AircraftMover(+playerSpeed, 0.f));

	for (auto& pair : mActionBinding) 
	{
		pair.second.category = static_cast<unsigned int>(Category::Type::PlayerAircraft);
	}
}

void Player::handleEvent(const sf::Event & event, CommandQueue & commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		//If pressed key is binded, trigger command
		auto found = mKeyBinding.find(event.key.code);
		if (found != mKeyBinding.end() && !isRealTimeAction(found->second)) 
		{
			commands.push(mActionBinding[found->second]);
		}
	}
}

void Player::handleRealTimeInput(CommandQueue& commands)
{
	for (auto pair : mKeyBinding) 
	{
		if (sf::Keyboard::isKeyPressed(pair.first)
			&& isRealTimeAction(pair.second)) 
		{
			commands.push(mActionBinding[pair.second]);
		}
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	for (auto binding : mKeyBinding)
	{
		if (binding.second == action) 
		{
			mKeyBinding.erase(binding.first);
		}
	}

	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	for (auto binding : mKeyBinding)
	{
		if (binding.second == action)
		{
			return binding.first;
		}
	}

	return sf::Keyboard::Unknown;
}

bool Player::isRealTimeAction(Action action)
{
	switch (action)
	{
	case Action::MoveUp:
	case Action::MoveDown:
	case Action::MoveLeft:
	case Action::MoveRight:
		return true;

	default:
		return false;
	}
}