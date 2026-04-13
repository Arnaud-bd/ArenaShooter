#pragma once
#include "Room/Room.h"

class Room_4 : public Room
{
private:
	void OnUpdate() override;
public:
	void OnInitRoom() override;
};