#include "Bullet.h"
#include "Tank.h"
#include "Map.h"
#pragma comment(lib,"winmm.lib")

CBullet::CBullet()
{
	m_core = { 0,0 };
	m_dir = UP;
	m_state = 不存在;
	m_who = -1;
}
void CBullet::SetBullet(CTank tank)
{
	m_core = tank.m_core;
	m_dir = tank.m_dir;
	m_who = tank.m_who;
}
void CBullet::MoveBullet()
{
	switch (m_dir)
	{
	case UP:
		m_core.Y--;
		break;
	case DOWN:
		m_core.Y++;
		break;
	case LEFT:
		m_core.X--;
		break;
	case RIGHT:
		m_core.X++;
		break;
	default:
		break;
	}
}
void CBullet::CleanBullet(COORD oldBulCore)
{
	GotoxyAndPrint(oldBulCore.X, oldBulCore.Y, "  ");
}
void CBullet::IsBulMeetOther(CTank tank, CTank* pMyTank, CTank* pEnemyTank, CMap& map)
{
	//遇边界
	if (m_core.X <= 0 || m_core.X >= MAP_X_WALL / 2 || m_core.Y <= 0 || m_core.Y >= MAP_Y - 1)
	{
		m_state = 不存在;
	}	
	//遇石块
	if (map.m_nArrMap[m_core.X][m_core.Y] == 石块)
	{
		m_state = 不存在;
	}
	//遇土块
	if (map.m_nArrMap[m_core.X][m_core.Y] == 土块)
	{
		m_state = 不存在;
		map.m_nArrMap[m_core.X][m_core.Y] = 空地;
	}
	//遇草丛

	//遇河流

	//遇子弹

	if (m_who != 敌方坦克)
	{
		//遇泉水
		if (map.m_nArrMap[m_core.X][m_core.Y] == 泉水)
			m_state = 不存在;
		// 遇我方其他
		for (int i = 0; i < MY_TANK_AMOUNT; i++)
		{
			// 死坦克不判断
			if (pMyTank[i].m_isAlive == false) continue;
			// 发出本子弹的坦克不判断（对比body而非core，因为子弹已经向前走了一步；C版本未判断但是正常也许某些实现不一样
			if (m_core.X == pMyTank[i].m_body[0].X && m_core.Y == pMyTank[i].m_body[0].Y) continue;
			if (
				(m_core.X == pMyTank[i].m_core.X) && (m_core.Y == pMyTank[i].m_core.Y) ||
				(m_core.X == pMyTank[i].m_body[0].X) && (m_core.Y == pMyTank[i].m_body[0].Y) ||
				(m_core.X == pMyTank[i].m_body[1].X) && (m_core.Y == pMyTank[i].m_body[1].Y) ||
				(m_core.X == pMyTank[i].m_body[2].X) && (m_core.Y == pMyTank[i].m_body[2].Y) ||
				(m_core.X == pMyTank[i].m_body[3].X) && (m_core.Y == pMyTank[i].m_body[3].Y) ||
				(m_core.X == pMyTank[i].m_body[4].X) && (m_core.Y == pMyTank[i].m_body[4].Y)
				)
			{
				m_state = 不存在;
			}
		}
		// 遇敌方坦克
		for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		{
			// 死坦克不判断
			if (pEnemyTank[i].m_isAlive == false) continue;
			if (
				(m_core.X == pEnemyTank[i].m_core.X) && (m_core.Y == pEnemyTank[i].m_core.Y) ||
				(m_core.X == pEnemyTank[i].m_body[0].X) && (m_core.Y == pEnemyTank[i].m_body[0].Y) ||
				(m_core.X == pEnemyTank[i].m_body[1].X) && (m_core.Y == pEnemyTank[i].m_body[1].Y) ||
				(m_core.X == pEnemyTank[i].m_body[2].X) && (m_core.Y == pEnemyTank[i].m_body[2].Y) ||
				(m_core.X == pEnemyTank[i].m_body[3].X) && (m_core.Y == pEnemyTank[i].m_body[3].Y) ||
				(m_core.X == pEnemyTank[i].m_body[4].X) && (m_core.Y == pEnemyTank[i].m_body[4].Y)
				)
			{
				PlaySound(TEXT("C:\\Users\\ry1yn\\source\\repos\\15PB\\Tank_cpp\\conf\\duang.wav"), NULL, SND_FILENAME | SND_ASYNC);//播放音效
				m_state = 不存在;
				pEnemyTank[i].m_blood--;
				if (pEnemyTank[i].m_blood == 0)//减血后为0则死亡
					pEnemyTank[i].m_isAlive = false;
				if (!pEnemyTank[i].m_isAlive && (ENEMY_TANK_AMOUNT - GetLiveEnemyAmount(pEnemyTank)) % 3 == 0)//每打死三个生命值+1
					(tank.m_blood)++;//要加!penemytank[i].isAlive，要不打到多条命的敌坦也加命
			}
		}
	}
	else if (m_who == 敌方坦克)
	{
		//遇泉水
		if (map.m_nArrMap[m_core.X][m_core.Y] == 泉水)
		{
			m_state = 不存在;
			pMyTank[0].m_blood = 0;//泉水打到，我方坦克当场去世
			pMyTank[1].m_blood = 0;//泉水打到，我方坦克当场去世
		}
		//遇到我方坦克
		for (int i = 0; i < MY_TANK_AMOUNT; i++)
		{
			// 死坦克不判断
			if (pMyTank[i].m_isAlive == false) continue;
			if (
				(m_core.X == pMyTank[i].m_core.X) && (m_core.Y == pMyTank[i].m_core.Y) ||
				(m_core.X == pMyTank[i].m_body[0].X) && (m_core.Y == pMyTank[i].m_body[0].Y ) ||
				(m_core.X == pMyTank[i].m_body[1].X) && (m_core.Y == pMyTank[i].m_body[1].Y ) ||
				(m_core.X == pMyTank[i].m_body[2].X) && (m_core.Y == pMyTank[i].m_body[2].Y ) ||
				(m_core.X == pMyTank[i].m_body[3].X) && (m_core.Y == pMyTank[i].m_body[3].Y ) ||
				(m_core.X == pMyTank[i].m_body[4].X) && (m_core.Y == pMyTank[i].m_body[4].Y )
				)
			{
				PlaySound(TEXT("conf/duang.wav"), NULL, SND_FILENAME | SND_ASYNC);//播放音效
				m_state = 不存在;
				(pMyTank[i].m_blood)--;
				if (pMyTank[i].m_blood == 0)//如果减血后为0
					pMyTank[i].m_isAlive = false;//声明为死亡
			}

		}
		//遇其他敌方坦克
		for (int i = 0; i < ENEMY_TANK_AMOUNT; i++)
		{
			// 死坦克不判断
			if (pEnemyTank[i].m_isAlive == false) continue;
			//发出本子弹的坦克不判断
			if (m_core.X == pEnemyTank[i].m_body[0].X && m_core.Y == pEnemyTank[i].m_body[0].Y) continue;

			if (
				(m_core.X == pEnemyTank[i].m_core.X) && (m_core.Y == pEnemyTank[i].m_core.Y) ||
				(m_core.X == pEnemyTank[i].m_body[0].X) && (m_core.Y == pEnemyTank[i].m_body[0].Y ) ||
				(m_core.X == pEnemyTank[i].m_body[1].X) && (m_core.Y == pEnemyTank[i].m_body[1].Y ) ||
				(m_core.X == pEnemyTank[i].m_body[2].X) && (m_core.Y == pEnemyTank[i].m_body[2].Y ) ||
				(m_core.X == pEnemyTank[i].m_body[3].X) && (m_core.Y == pEnemyTank[i].m_body[3].Y ) ||
				(m_core.X == pEnemyTank[i].m_body[4].X) && (m_core.Y == pEnemyTank[i].m_body[4].Y )
				)
			{
				m_state = 不存在;
			}

		}
	}
}
void CBullet::DrawBullet(CMap map)
{
	//碰到边界，换成边界的颜色，实现子弹消失的效果
	if (m_core.X <= 0 ||
		m_core.X >= MAP_X_WALL / 2 ||
		m_core.Y <= 0 ||
		m_core.Y >= MAP_Y - 1)
	{
		setColor(7, 0);
	}
	else
	{
		if (m_who != 敌方坦克)
			setColor(10, 0);
		else
			setColor(11, 0);
	}
	//碰到障碍，将子弹画为空格，实现子弹消失
	if (map.m_nArrMap[m_core.X][m_core.Y] == 土块)
	{
		GotoxyAndPrint(m_core.X, m_core.Y, "  ");
	}
	//碰到石块障碍物，，实现子弹消失的效果
	else if (map.m_nArrMap[m_core.X][m_core.Y] == 石块)
	{
		setColor(7, 0);
	}
	//碰到泉水，将子弹换成其颜色和形状，实现子弹消失
	else if (map.m_nArrMap[m_core.X][m_core.Y] == 泉水)
	{
		setColor(12, 0);
		GotoxyAndPrint(m_core.X, m_core.Y, "★");
	}
	//一般运动状态
	else
	{
		GotoxyAndPrint(m_core.X, m_core.Y, "■");
	}
	setColor(7, 0);
}