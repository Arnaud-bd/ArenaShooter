#ifndef ENGINE_STATES_SYSTEM_H_INCLUDED
#define ENGINE_STATES_SYSTEM_H_INCLUDED
#include "define.h"

namespace gce 
{

	struct StateMachine;
	class GameObject;

	class StatesSystem final
	{
	public :
		
		StateMachine* CreateStateMachine(GameObject* pMe);
		void DeleteStateMachine(GameObject* pMe);
		
	private :
		
		StatesSystem() = default;
		~StatesSystem() = default;
		void HandleStateMachines();
		void DestroyStateMachine(GameObject* pMe);

		Vector<GameObject*> m_deletionList;

		UnorderedMap<GameObject*, StateMachine*> m_stateMachineList;
		
		friend class GameManager;
	};

}
#endif
