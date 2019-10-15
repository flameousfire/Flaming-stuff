#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "pcards.hpp"
#include <stdlib.h>

struct Player;
struct Gameround;


static std::vector<Player> playerlist;


struct Player
{
    std::string name;
    int id;
    int stack;
    int bet;
    std::vector<int> hand;

Player()
{
    static int id = 0;
    ++id;
    name = "Player" + std::to_string(id);
    stack = 1000;
    bet = 0;
    dealcards(hand, 5);
}

};

struct Gameround
{
    int playersnum;
    int dealer;
    int pot;
    std::vector<Player*> roundplayers;
    int sblind;
    int bblind;
    int highestbet;
    int counter;
    int turnorder;

Gameround()
{
    static int round = 0;
    ++round;
    playersnum = playerlist.size();
    dealer = (2*playersnum - 4 + round) % playersnum;
    pot = 0;
    for (int i = 0; i < playersnum; ++i)
        roundplayers.push_back(&playerlist[i]);
    sblind = 50 * (1 + round / 5);
    bblind = 2 * sblind;
    smallblind(playerlist[(dealer + 1) % playersnum]);
    bigblind(playerlist[(dealer + 2) % playersnum]);
    highestbet = bblind;
    counter = 0;
    turnorder = dealer + 2;
}

void turncaller()
{
    ++turnorder;
    if (turnorder >= playersnum)
        turnorder %= playersnum;
    if (*roundplayers[turnorder]->stack = 0)
        continue;
    if (!((counter == playersnum && highestbet == 0) || (counter == playersnum - 1 && highestbet > bblind) || (highestbet == bblind && counter == playersnum)))
    {
        turn(*roundplayers[turnorder]);
    }
    else
    {
        turnorder = dealer;
        highestbet = 0;
        counter = 0;
        for (int i = 0; i < playerlist.size(); ++i)
        {
            pot += playerlist[i].bet;
            playerlist[i].bet = 0;
        }
    }
    
}

void smallblind(Player& player)
{
    player.stack -= sblind;
    player.bet = sblind;
    std::cout << player.name << " has the small blind. " << sblind  << "\n";
}

void bigblind(Player& player)
{
    player.stack -= bblind;
    player.bet = bblind;
    std::cout << player.name << " has the big blind. " << bblind  << "\n";

}

void raise(Player& who)
{
    std::cout << "How much will you raise?\n";
    int raise = 0;
    counter = 0;
    while (raise < bblind && raise != who.stack)
    {
        std::cin >> raise;
        if (raise < bblind)
        std::cout << "That raise is too small.\n";
    }
    int amount = highestbet - who.bet;
    if (who.stack < amount + raise)
    {
        std::cout << "All in!\n";
        who.bet += who.stack;
        who.stack = 0;
        if (who.bet > highestbet)
        highestbet = who.bet;
    }
    else
    {
    who.stack -= amount + raise;
    who.bet += amount + raise;
    highestbet = highestbet + raise;
    std::cout << "Raise " << raise << "!\n";
    }
}

void call(Player& who)
{
    int amount = highestbet - who.bet;
    ++counter;
    if (who.stack < amount)
    {
        std::cout << "All in!\n";
        who.bet = who.stack;
        who.stack = 0;
    }
    else
    {
        who.stack -= amount;
        who.bet += amount;
        if (amount == 0)
            std::cout << "Check. \n";
        else
            std::cout << "Call.\n"; 
    }
}


void fold(Player* who)
{
    std::cout << "I'm out. \n";
    std::vector<Player*>::iterator it = std::find(roundplayers.begin(), roundplayers.end(), who);
    roundplayers.erase(it);
    --playersnum;
    --turnorder;
}


void turn(Player& who)
{
    if (system("CLS")) system("clear");
    std::cout << "Players fighting for the pot: ";
    for (int i = 0; i < playersnum; ++i)
        std::cout << roundplayers[i]->name << " ";
    std::cout << "\n";
    printhand(who.hand);
    std::cout << "\n" << who.name << " to act.\n";
    int amount = highestbet - who.bet;
    std::cout << "Highest bid is " << highestbet << ". You must pay " << amount << " to call. \n";
    std::cout << "Choose to fold(f), call/check(c) or raise(r): \n";
    std::string choice = "";
    while (choice != "f" && choice != "c" && choice != "r")
        std::cin >> choice;
    if (choice == "f")
        fold(&who);
    else if (choice == "c")
        call(who);
    else
        raise(who);
    turncaller();
}

void comparetable()
{
    std::vector<int> besthand = roundplayers[0]->hand;
    std::vector<int> tiehand;
    int winnerindex = 0;
    int tieindex = 0;
    for (int i = 1; i < playersnum; ++i)
    {
        if (comparehands(besthand, roundplayers[i]->hand) == 2)
        {
            besthand = roundplayers[i]->hand;
            winnerindex = i;
        }
        else if (comparehands(besthand, roundplayers[i]->hand) == 0)
        {
            tiehand = roundplayers[i]->hand;
            tieindex = i;
        }
    }
    if (tieindex == 0)
    {
        potdivider(winnerindex);
    }

}

void potdivider(int win, int tie = 0)
{
    if (tie == 0)
    {
        roundplayers[win]->stack += pot;
        std::cout << roundplayers[win]->name << " wins with " << combprint(combination(roundplayers[win]->hand)) << "\n";
    }

    else
    {
        /* code */
    }
    
}

void winlosscheck()
{
    for (int i = 0; i < playerlist.size(); ++i)
    {
        if (playerlist[i].stack == 0)
        {
            std::cout << playerlist[i].name << " is out!\n";
            playerlist.erase(playerlist.begin() + i);
        }
    }
    
    if (playerlist.size() == 1)
        std::cout << playerlist[0].name << "has won the game! \n";
}

void roundrun()
{
    turncaller();
    for (int i = 0; i < roundplayers.size(); ++i)
    {
        std::cout << roundplayers[i]->name << "\n";
        printhand(roundplayers[i]->hand);
        cardchanger(roundplayers[i]->hand);
    }    
    turn(*roundplayers[dealer]);
    comparetable();
    
    for (int i = 0; i < roundplayers.size(); ++i)
    {
        printhand(roundplayers[i]->hand);
        std::cout << roundplayers[i]->stack << "\n";
    }
    winlosscheck();
}
};



int main()
{

for (int i = 1; i <= 52; ++i)
{    
    deck.push_back(i);
}


for (int i = 0; i < 4; ++i)
    playerlist.push_back(Player());

    while (playerlist.size() > 1)
    {
        std::cout << "New round begins! \n";
        Gameround round = Gameround();
        round.roundrun();
    }



}