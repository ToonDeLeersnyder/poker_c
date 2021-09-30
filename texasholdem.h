#ifndef TEXASHOLDEM_H
#define TEXASHOLDEM_H

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <algorithm>

#define nullptr NULL
#define STARTER_CHIPS 128
#define COMBO_SIZE 7
#define MAX_PLAYERS_IN_GAME 10
#define FULL_PRINT
//#define DEBUG_PRINT

typedef char byte;

typedef unsigned char RANK;

enum COMPARE
{
	LOWER = -1,
	EQUAL = 0,
	HIGHER = 1
};

enum SUIT
{
	SPADES = 0x6,
	DIAMONDS = 0x4,
	CLUBS = 0x5,
	HEARTS = 0x3
};

enum POKER_HAND
{
	HIGH_CARD = 0,
	ONE_PAIR,
	TWO_PAIR,
	THREE_OF_A_KIND,
	STRAIGHT,
	FLUSH,
	FULL_HOUSE,
	FOUR_OF_A_KIND,
	STRAIGHT_FLUSH
};

struct Card
{
	enum SUIT suit;
	RANK rank;
};

struct PokerRank
{
	enum POKER_HAND category;
	RANK rank;
	Card * hand[5];
};

struct Hand
{
	Card * cards[2];
	//Card * (*table)[5]; //Pointer to a table of pointers !
};

struct Player
{
	char name[40];
	byte ID = 0;
	struct Hand * hand;
	unsigned int chips = STARTER_CHIPS;
	unsigned int bet = 0;
};

/*struct ListOfPlayers
{
	struct Player * player;
	struct ListOfPlayers * next;
};*/

struct Game
{
	unsigned int plays = 0;
	byte dealer = 0;
	unsigned int blind = 1;
	unsigned int highestBet = 0;
	unsigned char amountHands = 0;
	Player * players[MAX_PLAYERS_IN_GAME];
	byte playersSize = 0;
	Card * deck[52];
	byte deckIndex = 0;
	//Card * (*table)[5];
};

struct showedCard
{
	Player * player;
	PokerRank rank;
	unsigned char placeAfterDealer = 0;
};

Card *table[5];

int willYouRaise( struct Game * game, struct Player * player, unsigned int totalBet ); //TO BE DEFINED !!!

void printCard( struct Card * card )
{
#if defined(_WIN32) || defined(__MSDOS__)
	printf( " %c", card->suit );
#else
	switch( card->suit )
	{
	case SPADES:
		printf( " \u2660" );
		break;
	case DIAMONDS:
		printf( " \u2666" );
		break;
	case CLUBS:
		printf( " \u2663" );
		break;
	case HEARTS:
		printf( " \u2665" );
		break;
	}
#endif
	switch( card->rank )
	{
	case 1:
	case 14:
		printf( " A" );
		break;
	case 11:
		printf( " J" );
		break;
	case 12:
		printf( " Q" );
		break;
	case 13:
		printf( " K" );
		break;
	default:
		printf( "%2d", card->rank );
		break;
	}
}

enum COMPARE compareCards( struct Card * lhs, struct Card * rhs )
{
	if( lhs->rank < rhs->rank )
	{
		return( LOWER );
	}
	else if( lhs->rank > rhs->rank )
	{
		return( HIGHER );
	}
	else
	{
		return( EQUAL );
	}
}

int handValue( struct PokerRank pokerHand )
{
	int returnValue = 0;
	for( byte i = 0 ; i < 5 ; i++ )
	{
		if( (Card *) pokerHand.hand[i] == nullptr )
		{
			printf( "Something went wrong..." );
			system( "pause" );
			return 0;
		}
		if( pokerHand.hand[i]->rank == 1 )
		{
			returnValue |= 14;
		}
		else
		{
			returnValue |= pokerHand.hand[i]->rank;
		}
		returnValue <<= 4;//4 bits for a card, 5 bytes is enough !
	}
	return( returnValue );
}

enum COMPARE comparePokerRanks( struct PokerRank lhs, struct PokerRank rhs )
{
	if( lhs.category < rhs.category || ( lhs.category == rhs.category && ( ( lhs.rank < rhs.rank && lhs.rank != 1 ) || ( lhs.rank == rhs.rank && handValue( lhs ) < handValue( rhs ) ) ) ) )
	{
		return( LOWER );
	}
	else if( lhs.category > rhs.category || ( lhs.category == rhs.category && ( ( lhs.rank > rhs.rank && rhs.rank != 1 ) || ( lhs.rank == rhs.rank && handValue( lhs ) > handValue( rhs ) ) ) ) )
	{
		return( HIGHER );
	}
	else
	{
		return( EQUAL );
	}
}

void printPokerRank( struct PokerRank pokerHand )
{
	switch( pokerHand.category )
	{
	case HIGH_CARD:
		printf( "High card " );
		break;
	case ONE_PAIR:
		printf( "One pair of " );
		break;
	case TWO_PAIR:
		printf( "Two pair with " );
		break;
	case THREE_OF_A_KIND:
		printf( "Three of a kind of " );
		break;
	case STRAIGHT:
		printf( "Straight from " );
		break;
	case FLUSH:
		printf( "Flush with " );
		break;
	case FULL_HOUSE:
		printf( "Full house with " );
		break;
	case FOUR_OF_A_KIND:
		printf( "Four of a kind of " );
		break;
	case STRAIGHT_FLUSH:
		printf( "Straight flush from " );
		break;
	}
	switch( pokerHand.rank )
	{
	case 1:
	case 14:
		printf( "A" );
		break;
	case 11:
		printf( "J" );
		break;
	case 12:
		printf( "Q" );
		break;
	case 13:
		printf( "K" );
		break;
	default:
		printf( "%d", pokerHand.rank );
		break;
	}
	printf( "\n" );
	for( size_t i = 0 ; i < 5 ; i++ )
	{
		if( pokerHand.hand[i] != nullptr )
		{
			printCard( pokerHand.hand[i] );
		}
	}
	printf( "\n\n" );
}

byte isHandFilled( struct Hand * hand )
{
	if( ( hand->cards[0] != nullptr ) && ( hand->cards[1] != nullptr ) )
	{
		return( 1 );
	}
	else
	{
		return( 0 );
	}
}

void clearHand( struct Hand * hand )
{
	hand->cards[0] = nullptr;
	hand->cards[1] = nullptr;
}

void addCardToHand( struct Hand * hand, struct Card * newCard )
{
	hand->cards[(hand->cards[0] == nullptr)?0:1] = newCard;
}

void printHand( struct Hand * hand )
{
	if( hand->cards[0] == nullptr )
	{
		printf( "Empty hand" );
	}
	else if( hand->cards[1] == nullptr )
	{
		printCard( hand->cards[0] );
	}
	else
	{
		printCard( hand->cards[0] );
		printCard( hand->cards[1] );
	}
	printf( "\n" );
}

PokerRank getMyHandRank( struct Hand * hand )
{
	//Prepping for help !
	struct Card * combo[COMBO_SIZE];
	byte comboSize = 0;
	if( hand != nullptr )
	{
		for( byte i = 0 ; i < 2 ; i++ ) //fill
		{
			if( hand->cards[i] != nullptr )
			{
				comboSize++;
			}
			combo[i] = hand->cards[i];
		}
	}
	for( byte i = 0 ; i < COMBO_SIZE - 2 ; i++ ) //fill
	{
		if( table[i] != nullptr )
		{
			comboSize++;
		}
		combo[i+2] = table[i];
	}
	for( byte i = 0 ; i < comboSize ; i++ ) //sort
	{
		if( combo[i] != nullptr )
		{
			for( byte j = i + 1 ; j < comboSize ; j++ )
			{
				if( combo[j] != nullptr && compareCards( combo[j], combo[i] ) == LOWER )
				{
					struct Card * temp = combo[i];
					combo[i] = combo[j];
					combo[j] = temp;
				}
			}
		}
	}

	PokerRank returnValue;
	//returnValue.hand = new Card[5];
	for( byte i = 0 ; i < 5 ; i++ )
	{
		returnValue.hand[i] = nullptr;
	}

	returnValue.category = HIGH_CARD;
	if( comboSize == 0 )
	{
		returnValue.rank = 0;
		return( returnValue );
	}
	if( comboSize == 1 )
	{
		if( combo[0]->rank == 1 )
		{
			returnValue.rank = 14; //Ace
		}
		else
		{
			returnValue.rank = combo[0]->rank;
		}
		returnValue.hand[0] = combo[0];
		return( returnValue );
	}
	if( comboSize == 2 )
	{
		if( combo[0]->rank == combo[1]->rank )
		{
			returnValue.category = ONE_PAIR;
		}
		if( combo[1]->rank == 1 )
		{
			returnValue.rank = 14; //Ace
		}
		else
		{
			returnValue.rank = combo[0]->rank;
		}
		returnValue.hand[0] = combo[0];
		returnValue.hand[1] = combo[1];
		return( returnValue );
	}

	byte suitCount[4] = {0,0,0,0};
	for( byte i = 0 ; i < comboSize ; i++ )
	{
		if( combo[i] != nullptr )
		{
			suitCount[combo[i]->suit-0x3]++;
		}
	}

	byte handIndex = 0;
	returnValue.rank = 0;
	//Checking options
	//STRAIGHT_FLUSH ?
	{
		for( byte s = 0x3 ; s <= 0x6 ; s++ ) //find first flush as 5 or more of the same cards...
		{
			if( suitCount[s-0x3] >= 5 )
			{ //FLUSH !
				//STRAIGHT_FLUSH ?
				byte sequencingNumbers = 1;
				RANK nextRankShouldBe = 0xFF; //for overflow !
				//Ace can be high AND low card !
				for( byte i = 0 ; i < comboSize && combo[i]->rank == 1 ; i++ ) //Check all Aces for the correct Suit
				{
					if( combo[i]->suit == s ) //Only correct
					{
						returnValue.hand[handIndex] = combo[i];
						handIndex++;
						nextRankShouldBe = 13; //We need a KING !
						returnValue.rank = 14; //Ace !
					}
				}
				for( byte i = comboSize - 1 ; i >= 0 ; i-- ) //check all card that have the same suit if they are in a subsequence, but from high to lower
				{
					if( combo[i]->suit == s ) //Only Flush Straight Check
					{
						if( combo[i]->rank == nextRankShouldBe ) //It is the next card we expected ?
						{
							returnValue.hand[handIndex] = combo[i];
							handIndex++;
							if( sequencingNumbers == 4 ) //We found the fifth subsequencing card, starting from the highest, so pack it an ship it !
							{
								returnValue.category = STRAIGHT_FLUSH;
								return( returnValue );
							}
							sequencingNumbers++;
						}
						else
						{
							handIndex = 0;
							returnValue.hand[handIndex] = combo[i];
							handIndex++;
							sequencingNumbers = 1; //Start all over as it is not the next card in line
							returnValue.rank = combo[i]->rank; //We start from the highest rank, that one we need to remember !
						}
						nextRankShouldBe = combo[i]->rank - 1; //We go from high to low
					}
				}
			}
		}
	}

	//FOUR_OF_A_KIND ?
	RANK valueOfPair = 0;
	RANK valueOfSecondPair = 0;
	RANK valueOfTrio = 0;
	{
		byte identicalNumbers = 0;
		handIndex = 0;
		for( byte i = 0 ; i < comboSize && combo[i]->rank == 1 ; i++ ) //Check all Aces first
		{
			returnValue.hand[handIndex] = combo[i];
			handIndex++;
			if( identicalNumbers == 1 )
			{
				valueOfPair = 14; //Pair of Aces
			}
			else if( identicalNumbers == 2 ) //No trio was found before, but this one is one !
			{
				valueOfTrio = 14; //Trio of Aces
				valueOfPair = 0; //not a pair...
			}
			else if( identicalNumbers == 3 ) //We found the fourth identical rank, starting from the highest, so pack it an ship it !
			{
				returnValue.category = FOUR_OF_A_KIND;
				returnValue.rank = 14; //Four Aces
				//Add highest card as kicker !
				if( combo[comboSize - 1]->rank != 1 ) //cannot be another Ace ! and if the last is Ace, there is no higher card.
				{
					returnValue.hand[handIndex] = combo[comboSize - 1];
					handIndex++;
				}
				return( returnValue );
			}
			identicalNumbers++;
		}
		handIndex = 0;
		returnValue.hand[handIndex] = combo[comboSize - 1];
		handIndex++;
		identicalNumbers = 1;
		returnValue.rank = combo[comboSize - 1]->rank; //start from the last card
		for( byte i = comboSize - 2 ; i >= 0 && combo[i]->rank != 1 ; i-- ) //chech all the oher cards for four of the same rank, from high to low
		{
			if( returnValue.rank == combo[i]->rank )
			{
				returnValue.hand[handIndex] = combo[i];
				handIndex++;
				if( valueOfPair == 0 ) //No pair was found before, but this one is one !
				{
					valueOfPair = combo[i]->rank; //Current rank is part of a pair
				}
				else if( valueOfSecondPair == 0 && valueOfPair != combo[i]->rank ) //No second pair was found before, but this one is one !
				{
					valueOfSecondPair = combo[i]->rank; //Current rank is part of a pair
				}
				if( identicalNumbers == 2 && valueOfTrio == 0 ) //No trio was found before, but this one is one !
				{
					valueOfTrio = combo[i]->rank; //Current rank is part of a trio
					if( valueOfPair == combo[i]->rank ) //clean out the pair that bacame a trio
					{
						valueOfPair = 0;
					}
					else if( valueOfSecondPair == combo[i]->rank )
					{
						valueOfSecondPair = 0;
					}
				}
				if( identicalNumbers == 3 ) //We found the fourth identical rank, starting from the highest, so pack it an ship it !
				{
					//Add highest card as kicker !
					if( combo[0]->rank == 1 ) //There is an Ace ! And we are not taking Aces in this part
					{
						returnValue.hand[handIndex] = combo[0];
						handIndex++;
					}
					else if( combo[comboSize - 1]->rank != combo[i]->rank ) //if the highest card is not one of the four, add this
					{
						returnValue.hand[handIndex] = combo[comboSize - 1];
						handIndex++;
					}
					else if( i > 0 ) //else if there is still another card, add that
					{
						returnValue.hand[handIndex] = combo[i-1];
						handIndex++;
					}
					returnValue.category = FOUR_OF_A_KIND;
					while( handIndex < 5 )
					{
						returnValue.hand[handIndex] = nullptr;
						handIndex++;
					}
					return( returnValue );
				}
				identicalNumbers++;
			}
			else
			{
				handIndex = 0;
				returnValue.hand[handIndex] = combo[i];
				handIndex++;
				identicalNumbers = 1; //Start all over as it is not the same card
				returnValue.rank = combo[i]->rank; //We start from the highest rank, that one we need to remember !
			}
		}
	}

	//FULL_HOUSE ?
	{
		if( valueOfTrio > 0 && valueOfPair > 0 )
		{
			returnValue.category = FULL_HOUSE;
			handIndex = 0;
			returnValue.rank = valueOfTrio;
			if( valueOfTrio == 14 ) //It was an Trio of Aces !
			{
				for( byte i = 0 ; i < comboSize && combo[i]->rank == 1 ; i++ )
				{
					returnValue.hand[handIndex] = combo[i]; //Add the Trio of Aces !
					handIndex++;
				}
			}
			else
			{
				for( byte i = comboSize - 1 ; i >= 0 ; i-- )
				{
					if( combo[i]->rank == valueOfTrio )
					{
						returnValue.hand[handIndex] = combo[i]; //Add the Trio
						handIndex++;
					}
				}
			}
			if( valueOfPair == 14 ) //Only the first pair can be Aces !
			{
				for( byte i = 0 ; i < comboSize && combo[i]->rank == 1 ; i++ )
				{
					returnValue.hand[handIndex] = combo[i]; //Add the first Pair of Aces !
					handIndex++;
					if( handIndex == 5 )
					{
						return( returnValue ); //we have 5 pack it, ship it !
					}
				}
			}
			else
			{
				for( byte i = comboSize - 1 ; i >= 0 ; i-- )
				{
					if( combo[i]->rank == valueOfPair )
					{
						returnValue.hand[handIndex] = combo[i]; //Add the Pair
						handIndex++;
						if( handIndex == 5 )
						{
							return( returnValue ); //we have 5 pack it, ship it !
						}
					}
				}
			}
			while( handIndex < 5 )
			{
				returnValue.hand[handIndex] = nullptr;
				handIndex++;
			}
			return( returnValue );
		}
	}

	//FLUSH ?
	{
		handIndex = 0;
		returnValue.rank = 0;
		for( byte s = 0x3 ; s <= 0x6 ; s++ ) //find first flush as 5 or more of the same cards...
		{
			if( suitCount[s-0x3] >= 5 )
			{ //FLUSH !
				returnValue.category = FLUSH;
				for( byte i = 0 ; i < comboSize && combo[i]->rank == 1 ; i++ ) //Check all Aces for the correct Suit
				{
					if( combo[i]->suit == s ) //Ace high flush !
					{
						returnValue.hand[handIndex] = combo[i];
						handIndex++;
						returnValue.rank = 14; //Ace !
					}
				}
				for( byte i = comboSize - 1 ; i >= 0 && combo[i]->rank != 1 ; i-- ) //In reverse order we will find the highest rank with the correct suit
				{
					if( combo[i]->suit == s ) //Only Flush Check
					{
						returnValue.hand[handIndex] = combo[i];
						handIndex++;
						if( returnValue.rank == 0 )
						{
							returnValue.rank = combo[i]->rank;
						}
						if( handIndex == 5 ) //Stop when we have the 5 highest cards of the same suit !
						{
							return( returnValue );
						}
					}
				}
			}
		}
	}

	//STRAIGHT?
	{
		byte sequencingNumbers = 1;
		RANK nextRankShouldBe = 0xFF; //for overflow !
		handIndex = 0;
		//Ace can be high AND low card !
		if( combo[0]->rank == 1 ) //If we have an Ace, it must be in the front
		{
			returnValue.hand[handIndex] = combo[0];
			handIndex++;
			nextRankShouldBe = 13; //We need a KING !
			returnValue.rank = 14; //Ace !
		}
		for( byte i = comboSize - 1 ; i >= 0 ; i-- ) //check all card if they are in a subsequence, but from high to lower
		{
			if( combo[i]->rank == nextRankShouldBe ) //It is the next card we expected ?
			{
				returnValue.hand[handIndex] = combo[i];
				handIndex++;
				if( sequencingNumbers == 4 ) //We found the fifth subsequencing card, starting from the highest, so pack it an ship it !
				{
					returnValue.category = STRAIGHT;
					return( returnValue );
				}
				sequencingNumbers++;
				nextRankShouldBe = combo[i]->rank - 1; //We go from high to low
			}
			else if( combo[i]->rank - 1 != nextRankShouldBe ) //The rank should not be the same as previous, else skip this card
			{
				handIndex = 0;
				returnValue.hand[handIndex] = combo[i];
				handIndex++;
				sequencingNumbers = 1; //Start all over as it is not the next card in line
				returnValue.rank = combo[i]->rank; //We start from the highest rank, that one we need to remember !
				nextRankShouldBe = combo[i]->rank - 1; //We go from high to low
			}
		}
	}

	//THREE_OF_A_KIND ?
	{
		if( valueOfTrio > 0 )
		{
			returnValue.category = THREE_OF_A_KIND;
			handIndex = 0;
			returnValue.rank = valueOfTrio;
			if( valueOfTrio == 14 ) //It was an Trio of Aces !
			{
				for( byte i = 0 ; i < comboSize && combo[i]->rank == 1 ; i++ )
				{
					returnValue.hand[handIndex] = combo[i]; //Add the Trio of Aces !
					handIndex++;
				}
			}
			else
			{
				for( byte i = comboSize - 1 ; i >= 0 ; i-- )
				{
					if( combo[i]->rank == valueOfTrio )
					{
						returnValue.hand[handIndex] = combo[i]; //Add the Trio
						handIndex++;
					}
				}
				for( byte i = 0 ; i < comboSize && combo[i]->rank == 1 ; i++ ) //First the Aces
				{
					returnValue.hand[handIndex] = combo[i]; //Add the highest rest
					handIndex++;
					if( handIndex == 5 )
					{
						return( returnValue ); //we have 5 pack it, ship it !
					}
				}
			}
			for( byte i = comboSize - 1 ; i >= 0 && combo[i]->rank != 1 ; i-- ) //No more Aces !
			{
				if( combo[i]->rank != valueOfTrio )
				{
					returnValue.hand[handIndex] = combo[i]; //Add the highest rest
					handIndex++;
					if( handIndex == 5 )
					{
						return( returnValue ); //we have 5 pack it, ship it !
					}
				}
			}
			while( handIndex < 5 )
			{
				returnValue.hand[handIndex] = nullptr;
				handIndex++;
			}
			return( returnValue );
		}
	}

	//TWO_PAIR ?
	{
		if( valueOfSecondPair > 0 )
		{
			returnValue.category = TWO_PAIR;
			handIndex = 0;
			returnValue.rank = valueOfPair;
			if( valueOfPair == 14 ) //Only the first pair can be Aces !
			{
				for( byte i = 0 ; i < comboSize && combo[i]->rank == 1 ; i++ )
				{
					returnValue.hand[handIndex] = combo[i]; //Add the first Pair of Aces !
					handIndex++;
				}
			}
			else
			{
				for( byte i = comboSize - 1 ; i >= 0 ; i-- )
				{
					if( combo[i]->rank == valueOfPair )
					{
						returnValue.hand[handIndex] = combo[i]; //Add the Pair
						handIndex++;
					}
				}
			}
			for( byte i = comboSize - 1 ; i >= 0 ; i-- )
			{
				if( combo[i]->rank == valueOfSecondPair )
				{
					returnValue.hand[handIndex] = combo[i]; //Add the second Pair
					handIndex++;
				}
			}
			if( valueOfPair != 14 )
			{
				for( byte i = 0 ; i < comboSize && combo[i]->rank == 1 ; i++ ) //First the Aces
				{
					returnValue.hand[handIndex] = combo[i]; //Add the highest rest
					handIndex++;
					if( handIndex == 5 )
					{
						return( returnValue ); //we have 5 pack it, ship it !
					}
				}
			}
			for( byte i = comboSize - 1 ; i >= 0 && combo[i]->rank != 1 ; i-- ) //No more Aces !
			{
				if( combo[i]->rank != valueOfPair && combo[i]->rank != valueOfSecondPair )
				{
					returnValue.hand[handIndex] = combo[i]; //Add the highest rest
					handIndex++;
					return( returnValue ); //we have 5 pack it, ship it !
				}
			}
			while( handIndex < 5 )
			{
				returnValue.hand[handIndex] = nullptr;
				handIndex++;
			}
			return( returnValue );
		}
	}

	//ONE_PAIR ?
	{
		if( valueOfPair > 0 )
		{
			returnValue.category = ONE_PAIR;
			handIndex = 0;
			returnValue.rank = valueOfPair;
			if( valueOfPair == 14 ) //Only the first pair can be Aces !
			{
				for( byte i = 0 ; i < comboSize && combo[i]->rank == 1 ; i++ )
				{
					returnValue.hand[handIndex] = combo[i]; //Add the first Pair of Aces !
					handIndex++;
				}
			}
			else
			{
				for( byte i = comboSize - 1 ; i >= 0 && combo[i]->rank != 1 ; i-- )
				{
					if( combo[i]->rank == valueOfPair )
					{
						returnValue.hand[handIndex] = combo[i]; //Add the Pair
						handIndex++;
					}
				}
				for( byte i = 0 ; i < comboSize && combo[i]->rank == 1 ; i++ ) //First the Aces
				{
					returnValue.hand[handIndex] = combo[i]; //Add the highest rest
					handIndex++;
					if( handIndex == 5 )
					{
						return( returnValue ); //we have 5 pack it, ship it !
					}
				}
			}
			for( byte i = comboSize - 1 ; i >= 0 && combo[i]->rank != 1 ; i-- )
			{
				if( combo[i]->rank != valueOfPair )
				{
					returnValue.hand[handIndex] = combo[i]; //Add the highest rest
					handIndex++;
					if( handIndex == 5 )
					{
						return( returnValue ); //we have 5 pack it, ship it !
					}
				}
			}
			while( handIndex < 5 )
			{
				returnValue.hand[handIndex] = nullptr;
				handIndex++;
			}
			return( returnValue );
		}
	}

	//HIGH_CARD !
	{
		returnValue.category = HIGH_CARD;
		handIndex = 0;
		if( combo[0]->rank == 1 ) //If we have an Ace, it must be in the front
		{
			returnValue.rank = 14; //Ace !
			returnValue.hand[handIndex] = combo[0];
			handIndex++;
		}
		else
		{
			returnValue.rank = combo[comboSize - 1]->rank; //Last card is the highest
		}
		for( byte i = comboSize - 1 ; i >= 0 && combo[i]->rank != 1 ; i-- )
		{
			returnValue.hand[handIndex] = combo[i]; //Add the highest rest
			handIndex++;
			if( handIndex == 5 )
			{
				return( returnValue ); //we have 5 pack it, ship it !
			}
		}
		while( handIndex < 5 )
		{
			returnValue.hand[handIndex] = nullptr;
			handIndex++;
		}
		return( returnValue );
	}
}

void clearPlayer( struct Player * player )
{
	clearHand( player->hand );
	player->bet = 0;
}

void playerFold( struct Player * player )
{
	clearHand( player->hand );
}

unsigned int playerPayUp( struct Player * player, unsigned int debts )
{
	debts = std::min( debts, player->bet );
	player->chips -= debts;
	player->bet -= debts;
	return( debts );
}

void playerWin( struct Player * player, unsigned int gain )
{
	player->chips += gain;
}

unsigned int playerRaiseBet( struct Player * player, unsigned int totalBet )
{
	player->bet = std::min( totalBet, player->chips );
	return( player->bet ); //Check if raise is possible ? Just take the minimum value !
}

void addPlayerToGame( struct Game * game, struct Player * newPlayer )
{
	/*for( byte i = 0 ; i < 5 ; i++ )
	{
		(*newPlayer->hand->table)[i] = (*game->table)[i];
	}*/
	//newPlayer->hand->table = game->table;
	newPlayer->hand = new Hand;
	clearPlayer( newPlayer );
	game->players[game->playersSize] = newPlayer;
	game->playersSize++;
}

void makeNewDeck( struct Game * game )
{
	srand( (unsigned int) time( NULL ) );
	unsigned char i;
	//game->deck = (Card**) malloc( sizeof( struct Card ) * 52 );
	for( i = 1 ; i <= 13 ; i++ )
	{
		game->deck[i-1] = new Card;
		game->deck[i-1]->suit = SPADES;
		game->deck[i-1]->rank = i;
	}
	for( i = 1 ; i <= 13 ; i++ )
	{
		game->deck[i+12] = new Card;
		game->deck[i+12]->suit = DIAMONDS;
		game->deck[i+12]->rank = i;
	}
	for( i = 1 ; i <= 13 ; i++ )
	{
		game->deck[i+25] = new Card;
		game->deck[i+25]->suit = CLUBS;
		game->deck[i+25]->rank = i;
	}
	for( i = 1 ; i <= 13 ; i++ )
	{
		game->deck[i+38] = new Card;
		game->deck[i+38]->suit = HEARTS;
		game->deck[i+38]->rank = i;
	}
}

void shuffleDeckInGame( struct Game * game )
{
	for( short i = 0 ; i < 1024 ; i++ )
	{
		byte a = rand() % 52;
		byte b = rand() % 52;
		struct Card * temp = game->deck[a];
		game->deck[a] = game->deck[b];
		game->deck[b] = temp;
	}
	game->deckIndex = 0;
}

void dealPocketInGame( struct Game * game )
{
	byte i = ( game->dealer + 1 ) % game->playersSize; //first card is give to player left from dealer
	while( isHandFilled( game->players[i]->hand ) == 0 )
	{
		addCardToHand( game->players[i]->hand, game->deck[game->deckIndex++] );
		i = ( i + 1 ) % game->playersSize; //next player on the left in round robin
	}
	game->amountHands = game->playersSize;
}

void bettingInGame( struct Game * game, byte isPreFlop )
{
	size_t i = ( game->dealer + 1 ) % game->playersSize; //first bet starts left from dealer
	if( isPreFlop > 0 )
	{
		game->highestBet = playerRaiseBet( game->players[i], game->blind ); //first small blind starts left from dealer
		i = ( i + 1 ) % game->playersSize; //next player on the left in round robin for big blind
		game->highestBet = std::max( game->highestBet, playerRaiseBet( game->players[i], game->blind * 2 ) );
		i = ( i + 1 ) % game->playersSize;
	}
	char minimumPlayerCheck = game->playersSize;
	int raisedBet;
	while( ( game->players[i]->bet < game->highestBet ) || ( game->amountHands > 1 && minimumPlayerCheck > 0 ) )
	{
		if( minimumPlayerCheck > 0 )
		{
			minimumPlayerCheck--;
		}
		if( isHandFilled( game->players[i]->hand ) > 0 && game->players[i]->chips != game->players[i]->bet ) //playing and not all in yet
		{
			raisedBet = willYouRaise( game, game->players[i], game->highestBet );
			if( raisedBet < 0 )
			{
				if( game->players[i]->bet < game->highestBet )
				{
					playerFold( game->players[i] );
					game->amountHands--;
				}
			}
			else
			{
				game->highestBet = std::max( game->highestBet, playerRaiseBet( game->players[i], game->highestBet + raisedBet ) );
			}
		}
		i = ( i + 1 ) % game->playersSize; //next player on the left in round robin
	}
}

void flopInGame( struct Game * game )
{
	game->deckIndex++; //Burn Card
	table[0] = game->deck[game->deckIndex++];
	table[1] = game->deck[game->deckIndex++];
	table[2] = game->deck[game->deckIndex++];
}

void turnInGame( struct Game * game )
{
	game->deckIndex++; //Burn Card
	table[3] = game->deck[game->deckIndex++];
}

void riverInGame( struct Game * game )
{
	game->deckIndex++; //Burn Card
	table[4] = game->deck[game->deckIndex++];
}

void showdownInGame( struct Game * game )
{
	//Pay up
	struct showedCard * showedCards[MAX_PLAYERS_IN_GAME];// = new showedCard[MAX_PLAYERS_IN_GAME];
	for( byte i = 0 ; i < MAX_PLAYERS_IN_GAME ; i++ )
	{
		showedCards[i] = nullptr;
	}
	byte showedCardsIndex = 0;
	for( byte i = 0 ; i < game->playersSize ; i++ )
	{
		if( isHandFilled( game->players[i]->hand ) > 0 )
		{
			showedCards[showedCardsIndex] = new showedCard;
			showedCards[showedCardsIndex]->player = game->players[i];
			showedCards[showedCardsIndex]->rank = getMyHandRank( game->players[i]->hand );
			showedCards[showedCardsIndex]->placeAfterDealer = ( game->playersSize + i - 1 - game->dealer ) % game->playersSize;
			showedCardsIndex++;
		}
	}
	if( game->amountHands != showedCardsIndex )
	{
		std::cerr << "SOMETHING BAD HAPPEND, THE GAME IS RIGGED !!!" << std::endl;
		//system( "pause" );
	}
	if( showedCardsIndex == 0 )
	{ //should NOT happen, but OK house wins :D
		std::cerr << "HOUSE WINS !!!" << std::endl;
		//system( "pause" );
	}
	else if( showedCardsIndex == 1 )
	{ //Bluff !
		unsigned int pot = 0;
		for( byte i = 0 ; i < game->playersSize ; i++ )
		{
			pot += playerPayUp( game->players[i], game->highestBet );
		}
		playerWin( showedCards[0]->player, pot );
#ifdef FULL_PRINT
		printf( "Player %s bluffed the others out and receives %d\n", showedCards[0]->player->name, pot );
#endif
	}
	else
	{
		for( byte i = 0 ; i < showedCardsIndex ; i++ ) //sort
		{
			for( byte j = i + 1 ; j < showedCardsIndex ; j++ )
			{
				enum COMPARE comp = comparePokerRanks( showedCards[i]->rank, showedCards[j]->rank );
				if( comp == HIGHER || ( comp == EQUAL && showedCards[i]->player->bet < showedCards[j]->player->bet ) )
				{
					struct showedCard * temp = showedCards[i];
					showedCards[i] = showedCards[j];
					showedCards[j] = temp;
				}
			}
		}
#ifdef FULL_PRINT
		printf( "%d players in showdown !\n", showedCardsIndex );
#endif
#ifdef DEBUG_PRINT
		for( byte i = 1 ; i <= showedCardsIndex ; i++ )
		{
			printf( "%s ", showedCards[showedCardsIndex-i]->player->name );
			printPokerRank( showedCards[showedCardsIndex-i]->rank );
		}
#endif
		while( game->highestBet > 0 && showedCardsIndex > 0 )
		{
			unsigned int pot = 0;
			unsigned int bet = showedCards[showedCardsIndex-1]->player->bet;
#ifdef DEBUG_PRINT
			printf( "Take bet %d of %s\n", showedCards[showedCardsIndex-1]->player->bet, showedCards[showedCardsIndex-1]->player->name );
#endif
			game->highestBet -= bet;
			byte i;
			for( i = 0 ; i < game->playersSize ; i++ )
			{
				pot += playerPayUp( game->players[i], bet );
			}
			for( i = 2 ; i <= showedCardsIndex && ( comparePokerRanks( showedCards[showedCardsIndex-i]->rank, showedCards[showedCardsIndex-1]->rank ) == EQUAL ) ; i++ )
				; //i will be the amount of showedCards that will need to split the pot
			i--; //we found the one that was not comform
			if( i > 1 )
			{
#ifdef FULL_PRINT
				printf( "%d players need to split the pot of %d\n", i, pot );
#endif
				unsigned int rest = pot % i;
				pot /= i;
				while( i > 0 )
				{
					playerWin( showedCards[showedCardsIndex-i]->player, pot );
#ifdef FULL_PRINT
					printf( "%s wins %d with ", showedCards[showedCardsIndex-i]->player->name, pot );
					printPokerRank( showedCards[showedCardsIndex-i]->rank );
#endif
					i--;
				}
				if( rest > 0 )
				{
					unsigned char placeAfterDealer = game->playersSize;
					Player * luckyBastard = nullptr;
					for( i = 1 ; i <= showedCardsIndex && ( comparePokerRanks( showedCards[showedCardsIndex-i]->rank, showedCards[showedCardsIndex-1]->rank ) == EQUAL ) ; i++ )
					{
						if( showedCards[showedCardsIndex-i]->placeAfterDealer < placeAfterDealer )
						{
							luckyBastard = showedCards[showedCardsIndex-i]->player;
							placeAfterDealer = showedCards[showedCardsIndex-i]->placeAfterDealer;
						}
					}
					if( luckyBastard != nullptr )
					{
						playerWin( luckyBastard, rest );
#ifdef FULL_PRINT
						printf( "%s is a lucky bastard and gets the rest (%d) of the split pot\n", luckyBastard->name, rest );
#endif
					} //if not found (what should not happen) house wins !
				}
			}
			else
			{
				playerWin( showedCards[showedCardsIndex-1]->player, pot );
#ifdef FULL_PRINT
				printf( "%s has highest hand and wins %d with ", showedCards[showedCardsIndex-1]->player->name, pot );
				printPokerRank( showedCards[showedCardsIndex-1]->rank );
#endif
			}
			while( showedCardsIndex > 0 && showedCards[showedCardsIndex-1]->player->bet == 0 )
			{
				delete showedCards[showedCardsIndex-1];
				showedCardsIndex--;
			}
		}
	}
}

void cleanUpGame( struct Game * game )
{
	//Clean up
	game->highestBet = 0;
	for( byte i = 0 ; i < 5 ; i++ )
	{
		table[i] = nullptr;
	}
	for( byte i = 0 ; i < game->playersSize ; i++ )
	{
		clearPlayer( game->players[i] );
		if( game->players[i]->chips == 0 )
		{
			if( i < game->dealer )
			{
				game->dealer--;
			}
			for( byte j = i+1 ; j < game->playersSize ; j++ )
			{
				game->players[j-1] = game->players[j]; //erasing by closing the gap
			}
			game->players[game->playersSize-1] = nullptr; //clear the last pointer
			game->playersSize--;
			i--;
		}
	}
	game->dealer = ( game->dealer + 1 ) % game->playersSize;
}


void printGame( struct Game * game )
{
	if( table[0] == nullptr )
	{
		printf( "Empty table\n\n" );
	}
	else
	{
		printf( "Table " );
		for( byte i = 0 ; i < 5 ; i++ )
		{
			if( table[i] != nullptr )
			{
				printCard( table[i] );
			}
		}
		printf( "\n\n" );
	}
	for( byte i = 0 ; i < game->playersSize ; i++ )
	{
		if( game->dealer == i )
		{
			printf( "o " );
		}
		else
		{
			printf( "  " );
		}
		printf( "(%d) Player %s\n[%d] --> %d\n", i+1, game->players[i]->name, game->players[i]->chips, game->players[i]->bet );
		printHand( game->players[i]->hand );
	}
	printf( "\n" );
}

void playGame( struct Game * game, byte winnerAmount )
{
	for( byte i = 0 ; i < 5 ; i++ )
	{
		table[i] = nullptr;
	}
	while( game->playersSize > winnerAmount )
	{
		game->plays++;
		if( game->plays % 32 == 0 ) //4 rounds with 8 players
		{
			game->blind *= 2; //double the blind
		}
		printf( "Play %d (%d)\n--------\n\n", game->plays, game->blind );
		shuffleDeckInGame( game );
		dealPocketInGame( game );
#ifdef DEBUG_PRINT
		printGame( game );
#endif
		bettingInGame( game, 1 );
#ifdef DEBUG_PRINT
		printGame( game );
#endif
		if( game->amountHands == 1 )
		{
#ifdef FULL_PRINT
			printGame( game );
#endif
			showdownInGame( game );
			cleanUpGame( game );
			continue;
		}
		flopInGame( game );
		bettingInGame( game, 0 );
#ifdef DEBUG_PRINT
		printGame( game );
#endif
		if( game->amountHands == 1 )
		{
#ifdef FULL_PRINT
			printGame( game );
#endif
			showdownInGame( game );
			cleanUpGame( game );
			continue;
		}
		turnInGame( game );
		bettingInGame( game, 0 );
#ifdef DEBUG_PRINT
		printGame( game );
#endif
		if( game->amountHands == 1 )
		{
#ifdef FULL_PRINT
			printGame( game );
#endif
			showdownInGame( game );
			cleanUpGame( game );
			continue;
		}
		riverInGame( game );
		bettingInGame( game, 0 );
#ifdef FULL_PRINT
		printGame( game );
#endif
		showdownInGame( game );
		cleanUpGame( game );
	}
	printGame( game );
}

#endif // TEXASHOLDEM_H
