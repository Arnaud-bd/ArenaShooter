#include "Room.h"

#include "Enemy.h"
#include "Player.h"

#include "ArenaShooterManager.h"
#include "Spider1.h"

#include "Pathfinding/UtilsPathfinding.h"
#include "Pathfinding/Tile.h"
#include "Pathfinding/RoomConfiguration.h"

#include "MapLoader.hpp"

#define TEST true

void Room::SetNeighbors(Node<Tile>* node)
{
    int x = node->data->mPos.col;
    int y = node->data->mPos.row;
    int z = node->data->mPos.height;

    std::vector<Node<Tile>*> verifie;
    std::vector<Node<Tile>*> diagoVerifie;

    std::vector<int> ligneVerifie;

    if (z - 1 >= 0)
        ligneVerifie.push_back(-1);

    ligneVerifie.push_back(0);

    if (z + 1 < mCurrentDepth)
        ligneVerifie.push_back(1);

    for (int offsetZ : ligneVerifie)
    {
        if (y + 1 < mCurrentHeight)
            verifie.push_back(&m_vNodes[z + offsetZ][y + 1][x]);
        if (y - 1 >= 0)
            verifie.push_back(&m_vNodes[z + offsetZ][y - 1][x]);
        if (x + 1 < mCurrentWidth)
            verifie.push_back(&m_vNodes[z + offsetZ][y][x + 1]);
        if (x - 1 >= 0)
            verifie.push_back(&m_vNodes[z + offsetZ][y][x - 1]);
    }

    // DIAGONAAAAAAAAAAAAAAAAAAALE
    if (y - 1 >= 0 && x - 1 >= 0)
        diagoVerifie.push_back(&m_vNodes[z][y - 1][x - 1]);
    else
        diagoVerifie.push_back(nullptr);
    if (y - 1 >= 0 && x + 1 < mCurrentWidth)
        diagoVerifie.push_back(&m_vNodes[z][y - 1][x + 1]);
    else
        diagoVerifie.push_back(nullptr);
    if (y + 1 < mCurrentHeight && x + 1 < mCurrentWidth)
        diagoVerifie.push_back(&m_vNodes[z][y + 1][x + 1]);
    else
        diagoVerifie.push_back(nullptr);
    if (y + 1 < mCurrentHeight && x - 1 >= 0)
        diagoVerifie.push_back(&m_vNodes[z][y + 1][x - 1]);
    else
        diagoVerifie.push_back(nullptr);


    for (Node<Tile>* neighbor : verifie)
    {
        Tile* tile = neighbor->data;

        if (tile->mIsObstacle)
        {
            continue;
        }

        //verifier si il n'y a rien aus dessus
        if (tile->mPos.z + 1 < mCurrentDepth)
        {
            if (m_vNodes[tile->mPos.z + 1][tile->mPos.y][tile->mPos.x].data->mIsObstacle == false)
                continue;
        }

        node->neighbors.push_back(neighbor);
    }

    for (Node<Tile>* diagoNeighbor : diagoVerifie)
    {
        if (diagoNeighbor != nullptr)
        {
            Tile* tile = diagoNeighbor->data;
            //verifier si il n'y a rien aus dessus
            if (tile->mPos.z + 1 < mCurrentDepth)
            {
                if (m_vNodes[tile->mPos.z + 1][tile->mPos.y][tile->mPos.x].data->mIsObstacle == false)
                    diagoNeighbor = nullptr;
            }
        }

        node->diagoNeighbors.push_back(diagoNeighbor);
    }
}

Tile* Room::GetTileWithPos(gce::Vector3f32 pos)
{
    for (std::vector<std::vector<Node<Tile>>>& plan : m_vNodes)
    {
        for (std::vector<Node<Tile>>& ligne : plan)
        {
            for (Node<Tile>& node : ligne)
            {
                Tile* tile = node.data;

                if (UtilsPathfinding::IsInside(tile->mPos, pos))
                {
                    return tile;
                }
            }
        }
    }

    return nullptr;
}

void Room::ResetByEntity(Entity* pEntity)
{
    for (std::vector<std::vector<Node<Tile>>>& plan : m_vNodes)
    {
        for (std::vector<Node<Tile>>& ligne : plan)
        {
            for (Node<Tile>& node : ligne)
            {
                if (node.data->mEntity == pEntity)
                {
                    node.data->SetTempLock(false, nullptr);
                }
            }
        }
    }
}

void Room::UpdateNode(Node<Tile>* node, bool becomeObstacle)
{
    Tile* tile = node->data;

    if (becomeObstacle)
    {
        tile->mIsObstacle = true;

        for (Node<Tile>* neighbor : node->neighbors)
        {
            for (int i = 0; i < neighbor->neighbors.size(); i++)
            {
                if (neighbor->neighbors[i] == node)
                {
                    neighbor->neighbors.erase(neighbor->neighbors.begin() + i);
                    continue;
                }
            }
        }

        //mettre a jour celui en dessous si il existe
        if (node->data->mPos.z - 1 >= 0)
        {
            Node<Tile>* currentNode = &m_vNodes[tile->mPos.z - 1][tile->mPos.y][tile->mPos.x];

            SetNeighbors(currentNode);

            for (Node<Tile>* neighbor : currentNode->neighbors)
            {
                neighbor->neighbors.push_back(currentNode);
            }
        }
    }
    else
    {
        tile->mIsObstacle = false;
        node->neighbors.clear();

        SetNeighbors(node);

        for (Node<Tile>* neighbor : node->neighbors)
        {
            neighbor->neighbors.push_back(node);
        }

        //mettre a jour celui en dessous si il existe
        if (node->data->mPos.z - 1 >= 0)
        {
            Node<Tile>* currentNode = &m_vNodes[tile->mPos.z - 1][tile->mPos.y][tile->mPos.x];

            for (Node<Tile>* neighbor : currentNode->neighbors)
            {
                for (int i = 0; i < neighbor->neighbors.size(); i++)
                {
                    if (neighbor->neighbors[i] == currentNode)
                    {
                        neighbor->neighbors.erase(neighbor->neighbors.begin() + i);
                        continue;
                    }
                }
            }
        }
    }
}

Tile* Room::CreateTile()
{
    Tile* newTile = new Tile();
    newTile->OnInitialize();

    return newTile;
}

void Room::InitMap(std::vector<std::vector<std::vector<char>>> vMaps)
{
    int size = 1;

    int z = 0;
    for (std::vector<std::vector<char>> twoD : vMaps)
    {
        std::vector<std::vector<Tile*>> vectorTemp;
        int y = 0;
        for (std::vector<char> oneD : twoD)
        {
            std::vector<Tile*> temp;
            int x = 0;
            for (char box : oneD)
            {
                Tile* newTile = CreateTile();
                newTile->SetPosition(x, y, z);
                WPosMPos pos = { y, x, z, x, z, y };
                newTile->mPos = pos;

                if (box == '#')
                {
                    newTile->mIsObstacle = true;
                    newTile->SetColor(gce::Color::Black);
                    newTile->SetToDraw(false);
                }

                temp.push_back(newTile);

                x++;
            }

            vectorTemp.push_back(temp);
            y++;
        }

        m_vTiles.push_back(vectorTemp);
        z++;
    }

    // Initialisation des nodes
    for (const std::vector<std::vector<Tile*>>& plan : m_vTiles)
    {
        std::vector<std::vector<Node<Tile>>> temp2D;

        for (const std::vector<Tile*>& ligne : plan)
        {
            std::vector<Node<Tile>> temp1D;

            for (Tile* tile : ligne)
            {
                Node<Tile> node;
                node.data = tile;
                temp1D.push_back(node);
            }

            temp2D.push_back(temp1D);
        }

        m_vNodes.push_back(temp2D);
    }

    // Initialisation des voisins
    for (int z = 0; z < mCurrentDepth; z++)
    {
        for (int y = 0; y < mCurrentHeight; y++)
        {
            for (int x = 0; x < mCurrentWidth; x++)
            {
                Node<Tile>* currentNode = &m_vNodes[z][y][x];

                SetNeighbors(currentNode);
            }
        }
    }
}

void Room::InitRoom()
{
	m_pScene = ArenaShooterManager::GetInstance()->GetScene();

    m_pPlayer = ArenaShooterManager::GetInstance()->GetPlayer();

    OnInitRoom();

    m_pPlayer->GetOwner()->transform.SetWorldPosition(mPlayerStartPos);
}

void Room::DestroyRoom()
{
    for (GameObject* go : m_vGameObject)
    {
        go->Destroy();
    }

    for (std::vector<std::vector<Tile*>> x : m_vTiles)
    {
        for (std::vector<Tile*> y : x)
        {
            for (Tile* tile : y)
            {
                delete tile;
            }
        }
    }

    for (Enemy* enemy : m_vEnemy)
    {
        enemy->GetOwner()->Destroy();
    }

	delete this;
}

void Room::AddEnemy(Enemy* pEnemy)
{
    m_vEnemy.push_back(pEnemy);
}

void Room::DeleteEnemy(Enemy* pEnemy)
{
    int i = 0;
    for (Enemy* enemy : m_vEnemy)
    {
        if (enemy == pEnemy)
        {
            m_vEnemy.erase(m_vEnemy.begin() + i);
            pEnemy->GetOwner()->Destroy();
            return;
        }

        i++;
    }
}

bool Room::IsFinish()
{
	return m_vEnemy.size() == 0 || TEST;
}

void Room::ResetNodes()
{
	for (std::vector<std::vector<Node<Tile>>>& plan : m_vNodes)
	{
		for (std::vector<Node<Tile>>& ligne : plan)
		{
			for (Node<Tile>& node : ligne)
			{
				node.isVisited = false;
				node.comeFrom = nullptr;
				node.totalCost = 0;
				node.costFromStart = 0;
			}
		}
	}
}

gce::Vector3f32 Room::GetPosInTab(gce::Vector3f32 pos)
{
	Tile* tile = GetTileWithPos({ pos.x, pos.y, pos.z });

	if (tile != nullptr)
		return gce::Vector3f32(tile->mPos.col, tile->mPos.row, tile->mPos.height);

	return { 0, 0, 0 };
}

Node<Tile>* Room::GetNodeWithPos(gce::Vector3f32 pos)
{
	for (std::vector<std::vector<Node<Tile>>>& plan : m_vNodes)
	{
		for (std::vector<Node<Tile>>& ligne : plan)
		{
			for (Node<Tile>& node : ligne)
			{
				Tile* tile = node.data;

				if (UtilsPathfinding::IsInside(tile->mPos, {pos.x, 0, pos.z}))
					return &node;
			}
		}
	}

	return nullptr;
}

Node<Tile>* Room::GetNodeWithTabPos(gce::Vector3f32 pos)
{
	return &m_vNodes[pos.z][pos.y][pos.x];
}

void Room::Update()
{
	OnUpdate();

    if (m_pPlayer->GetOwner()->transform.GetWorldPosition().y <= -5)
    {
        m_pPlayer->GetOwner()->transform.SetWorldPosition(mPlayerStartPos + Vector3f32(0.f, 10.f, 0.f));
    }
}

void Room::RemoveGameObject(GameObject* gameObject)
{
    for (int i = 0; i < m_vGameObject.size(); i++)
    {
        if (m_vGameObject[i] == gameObject)
        {
            m_vGameObject.erase(m_vGameObject.begin() + i);
            return;
        }
    }
}

void Room::ActiveAllSpider()
{
    for (Enemy* enemy : m_vEnemy)
    {
        enemy->StartFight();
    }
}