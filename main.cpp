#include <iostream>
#include <string.h>
#define DEBUG_PRINT
#include "texasholdem.h"

int totalPlayers = 0;

int meineFunktion( struct Game * game, struct Player * player, unsigned int totalBet )
{
	PokerRank myCombo = getMyHandRank( player->hand );
	printf( "My rank : " );
	printPokerRank( myCombo );
//	for( int i = 0 ; i < game->playersSize ; i++ )
//	{
//		if( game->players[i]->ID != player->ID && isHandFilled( game->players[i]->hand ) )
//		{
//			PokerRank otherCombo = getMyHandRank( game->players[i]->hand );
//			printf( "His/Her rank : " );
//			printPokerRank( otherCombo );
//			if( comparePokerRanks( myCombo, otherCombo ) == LOWER )
//			{
//				printf( "Fuck it !\n" );
//				return( -1 );
//			}
//		}
//	}
	if( myCombo.category >= FLUSH )
	{
		return( player->chips );
	}
	if( table[0] == nullptr ) //nog geen kaarten op tafel
	{
		if( totalBet > ( player->bet * 2 ) || ( totalBet > ( game->blind * 5 ) && totalBet > ( player->chips / 2 ) ) )
		{
			return( -1 );
		}
		return( 0 );
	}
	if( table[4] == nullptr ) //er liggen maar 3 of 4 kaarten op tafel
	{
		if( myCombo.category >= TWO_PAIR )
		{
			int gewensteBet = player->chips / 4;
			if( gewensteBet > totalBet )
			{
				return( gewensteBet - totalBet );
			}
			return( 0 );
		}
		if( totalBet > ( player->bet * 2 ) || totalBet > ( player->chips / 2 ) )
		{
			return( -1 );
		}
		return( 0 );
	}
	if( table[4] != nullptr ) //er liggen 5 kaarten op tafel
	{
		if( myCombo.category >= TWO_PAIR )
		{
			int gewensteBet = player->chips / 4;
			if( gewensteBet > totalBet )
			{
				return( gewensteBet - totalBet );
			}
			return( 0 );
		}
		return( 0 );
	}
	return( 0 );
}

int willYouRaise( struct Game * game, struct Player * player, unsigned int totalBet)
{
	printf( "%s what do you do ?\n", player->name );
	switch( player->ID )
	{
	case 0: //THIS IS ME !!!!!!!!!!!!
	{
        int maxInzet = game->players[game->dealer]->chips / 2;
        PokerRank myCombo = getMyHandRank( player->hand );
        if( table[0] == nullptr && player->hand->cards[0]->rank >= 10 && player->hand->cards[1]->rank >= 10){
            if( myCombo.category >= ONE_PAIR  )
            {
                return( player->chips );
            }
            else {
                return( -1 );
            }
        }
        if( table[1] != nullptr && player->hand->cards[0]->rank >= 10 && player->hand->cards[1]->rank >= 10){

                if( myCombo.category >= THREE_OF_A_KIND )
                {
                    return( maxInzet );
                }
                else {
                    return( -1 );
                }
        }
        else {return ( -1); }

	}
	case 1: //Alice
	{
		if( player->hand->cards[0]->rank >= 10 || player->hand->cards[1]->rank >= 10 )
		{
			return( 10 );
		}
		return( -1 );
	}
	case 2: //Bob
	{
		PokerRank myCombo = getMyHandRank( player->hand );
		if( myCombo.category >= ONE_PAIR )
		{
			return( player->chips );
		}
		return( -1 );
	}
	case 3: //Carla
	{
		for( int i = 0 ; i < game->playersSize ; i++ )
		{ //search Alice
			if( game->players[i]->ID == 1 )
			{
				if( game->players[i]->bet > ( game->blind * 2 ) )
				{
					return( -1 );
				}
				else
				{
					return( 1 );
				}
			}
		}
		return( player->chips );
	}
	case 4: //Danny
	{
		int maxInzet = game->players[game->dealer]->chips / 2;
		if( maxInzet > totalBet )
		{
			return( maxInzet - totalBet );
		}
		return( 0 );
	}
	case 5: //Eric
	{
		if( game->playersSize > totalPlayers )
		{
			totalPlayers = game->playersSize;
		}
		if( totalPlayers > game->playersSize )
		{
			return( rand() % player->chips );
		}
		else
		{
			return( -1 );
		}
	}
	case 6 :
		return meineFunktion( game, player, totalBet );
	default:
		return( 0 ); //Call all the time
		break;
	}
	return( 0 );
}

int main( void )
{
	Game game;
	makeNewDeck( &game );

	Player p1;
	strcpy( p1.name, "Alice" );
	p1.ID = 1;

	Player p2;
	strcpy( p2.name, "Bob" );
	p2.ID = 2;

	Player p3;
	strcpy( p3.name, "Carla" );
	p3.ID = 3;

	Player p4;
	strcpy( p4.name, "Danny" );
	p4.ID = 4;

	Player p5;
	strcpy( p5.name, "Eric" );
	p5.ID = 5;

	Player p6;
	strcpy( p6.name, "Fuhrer" );
	p6.ID = 6;

	Player mijnSpeler;
	strcpy( mijnSpeler.name, "THIS IS ME <-----------------" );
	mijnSpeler.ID = 0;

	addPlayerToGame( &game, &p1 );
	addPlayerToGame( &game, &p2 );
	addPlayerToGame( &game, &p3 );
	addPlayerToGame( &game, &p4 );
	addPlayerToGame( &game, &p5 );
	addPlayerToGame( &game, &p6 );
	addPlayerToGame( &game, &mijnSpeler );

	playGame( &game, 1 );
	printf( "The winner is %s with %d chips !", game.players[0]->name, game.players[0]->chips );
	return 0;
}

