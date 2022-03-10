
#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <vector>
#include <map>
#include <string.h>
#include <math.h>

#define GATE_A  0x0001
#define GATE_B  0x0002
#define GATE_C  0x0004
#define GATE_D  0x0008
#define GATE_E  0x0010
#define GATE_F  0x0020
#define GATE_G  0x0040
#define GATE_H  0x0080

// #define DEBUG	1

/************************ Immune Status ************************/

enum ImmuneStatus
{
	UNAFFECTED,			// Not affected from the virus
	RECOVER,			// Can be recovered
	DIE					// Can not be recovered, will be died
};

/************************ Position Struct ************************/

struct Position
{
public:
	Position();
	Position(int, int);
	~Position();

public:
	int x;
	int y;
};

Position::Position()
{
	this->x = 0;
	this->y = 0;
}

Position::Position(int x, int y) : x(x), y(y)
{
}

Position::~Position()
{
}

/************************ Entity Class ************************/

class Entity
{
public:
	Entity();
	~Entity();

	void setId(int id);
	int getId();

	void setGate(int gate);
	bool hasGate(int gate);
	int getGates();

	void setInfected(bool is_infected);
	bool isInfected();

	void increaseInfectedDay();
	void resetInfectedDay();
	int getInfectedDay();

	void setIncubation(bool is_incubation);
	bool isIncubation();

	void increaseIncubationDay();
	void resetIncubationDay();
	int getIncubationDay();

	void setAlive(bool is_alive);
	bool isAlive();

	void setRecovered(bool is_recovered);
	bool isRecovered();

	void setPosition(Position& pos);
	void setPosition(Position&& pos);
	Position& getPosition();

	void addPosition(int x, int y);
	void addPosition(Position& pos);
	void move(int day);
	
	bool canInfect(Entity& entity);

	void setImmuneStatus(ImmuneStatus immune_status);
	ImmuneStatus getImmuneStatus();

private:
	int id;
	int gates;
	bool is_alive;
	bool is_infected;
	int infected_day;
	bool is_incubation;
	int incubation_day;
	bool is_recovered;
	ImmuneStatus immune_status;
	Position position;
	std::vector<Position> positions;
};

Entity::Entity()
{
	this->id = 0;
	this->gates = 0;
	this->is_alive = true;
	this->is_infected = false;
	this->infected_day = 0;
	this->is_incubation = false;
	this->incubation_day = 0;
	this->is_recovered = false;
	this->immune_status = UNAFFECTED;
}

Entity::~Entity()
{
}

void Entity::setId(int id)
{
	this->id = id;
}

int Entity::getId()
{
	return this->id;
}

void Entity::setGate(int gate)
{
	this->gates |= gate;
}

bool Entity::hasGate(int gate)
{
	return (this->gates & gate) != 0;
}

int Entity::getGates()
{
	return this->gates;
}

void Entity::setInfected(bool is_infected)
{
	this->is_infected = is_infected;
}

bool Entity::isInfected()
{
	return this->is_infected;
}

void Entity::increaseInfectedDay()
{
	this->infected_day++;
}

void Entity::resetInfectedDay()
{
	this->infected_day = 0;
}

int Entity::getInfectedDay()
{
	return this->infected_day;
}

void Entity::setIncubation(bool is_incubation)
{
	this->is_incubation = is_incubation;
}

bool Entity::isIncubation()
{
	return this->is_incubation;
}

void Entity::increaseIncubationDay()
{
	this->incubation_day++;
}

void Entity::resetIncubationDay()
{
	this->incubation_day = 0;
}

int Entity::getIncubationDay()
{
	return this->incubation_day;
}

void Entity::setAlive(bool is_alive)
{
	this->is_alive = is_alive;
}

bool Entity::isAlive()
{
	return this->is_alive;
}

void Entity::setRecovered(bool is_recovered)
{
	this->is_recovered = is_recovered;
}

bool Entity::isRecovered()
{
	return this->is_recovered;
}

void Entity::setPosition(Position& pos)
{
	this->position.x = pos.x;
	this->position.y = pos.y;
}

void Entity::setPosition(Position&& pos)
{
	this->position.x = pos.x;
	this->position.y = pos.y;
}

Position& Entity::getPosition()
{
	return this->position;
}

void Entity::addPosition(int x, int y)
{
	this->positions.push_back(Position(x, y));
}

void Entity::addPosition(Position& pos)
{
	this->positions.push_back(pos);
}

void Entity::move(int day)
{
	if (this->is_alive) {
		Position& pos = this->positions[day];

		this->position.x = pos.x;
		this->position.y = pos.y;
	}
}

bool Entity::canInfect(Entity& entity)
{
	if (this->is_infected && entity.is_alive && (entity.immune_status != UNAFFECTED) && !entity.is_recovered && !entity.is_infected && !entity.is_incubation) {
		if (std::abs(position.x - entity.position.x) <= 3 && std::abs(position.y - entity.position.y) <= 3)
			return true;
	}

	return false;
}

void Entity::setImmuneStatus(ImmuneStatus immune_status)
{
	this->immune_status = immune_status;
}

ImmuneStatus Entity::getImmuneStatus()
{
	return this->immune_status;
}

/************************ Game Class ************************/

class Game
{
public:
	Game();
	
	void init(const char *input, const char *output, const char *turn);
	bool parse();

	void printReport();
	void printTurn(int turn);
	void run();

private:
	int size;
	int turn_count;
	std::vector<Entity> entities;

	char *input_filename;
	char *output_filename;
	char *turn_filename;
};

// Constructor of Game class
Game::Game()
{
	this->size = 0;
	this->turn_count = 0;
	this->input_filename = NULL;
	this->output_filename = NULL;
	this->turn_filename = NULL;
}

// Initialize Game
void Game::init(const char *input, const char *output, const char *turn)
{
	this->input_filename = (char *) input;
	this->output_filename = (char *) output;
	this->turn_filename = (char *) turn;

	parse();
}

// Read and parse input file 
bool Game::parse()
{
	std::ifstream ifs(input_filename);

	if (ifs.is_open()) {
		std::string line;
		std::regex reg("\\s+");

		while (std::getline(ifs, line)) {
			std::sregex_token_iterator iter(line.begin(), line.end(), reg, -1);
			std::sregex_token_iterator end;

			std::vector<std::string> vec(iter, end);

			if (vec[0] == "size") {
				this->size = std::atoi(vec[1].c_str());
			}
			else if (vec[0] == "turn_count") {
				this->turn_count = atoi(vec[1].c_str());
			}
			else if (vec[0] == "entity") {
				Entity entity;

				entity.setId(std::atoi(vec[1].c_str()));

				for (size_t i = 0; i < vec[2].length(); i++) {
					switch (vec[2][i]) {
						case 'A':
						case 'a':
							entity.setGate(GATE_A);
							break;
						case 'B':
						case 'b':
							entity.setGate(GATE_B);
							break;
						case 'C':
						case 'c':
							entity.setGate(GATE_C);
							break;
						case 'D':
						case 'd':
							entity.setGate(GATE_D);
							break;
						case 'E':
						case 'e':
							entity.setGate(GATE_E);
							break;
						case 'F':
						case 'f':
							entity.setGate(GATE_F);
							break;
						case 'G':
						case 'g':
							entity.setGate(GATE_G);
							break;
						case 'H':
						case 'h':
							entity.setGate(GATE_H);
							break;
					}
				}

				if (entity.hasGate(GATE_A) || entity.hasGate(GATE_B)) {
					int total = entity.hasGate(GATE_A) + entity.hasGate(GATE_B) + entity.hasGate(GATE_C) + entity.hasGate(GATE_D);

					if (total >= 3)
						entity.setImmuneStatus(DIE);			// Can not be recovered, will be died
					else
						entity.setImmuneStatus(RECOVER);		// Can be recovered
				}
				else {
					entity.setImmuneStatus(UNAFFECTED);			// Not affected from the virus
				}

				std::string sx(vec[3].begin(), std::find(vec[3].begin(), vec[3].end(), 'x'));
				std::string sy(std::find(vec[3].begin(), vec[3].end(), 'x') + 1, vec[3].end());

				Position pos(std::atoi(sx.c_str()), std::atoi(sy.c_str()));

				entity.setPosition(pos);
				entity.addPosition(pos);

				if (vec.size() > 4 && vec[4] == "infected") {
					entity.setInfected(true);
					entity.increaseInfectedDay();
					entity.increaseInfectedDay();
					entity.increaseInfectedDay();
				}

				entities.push_back(entity);
			}
			else if (vec[0] == "turn") {
				int id = std::atoi(vec[1].c_str());

				for (size_t i = 2; i < vec.size(); i++) {
					std::string sx(vec[i].begin(), std::find(vec[i].begin(), vec[i].end(), 'x'));
					std::string sy(std::find(vec[i].begin(), vec[i].end(), 'x') + 1, vec[i].end());

					Position pos(std::atoi(sx.c_str()), std::atoi(sy.c_str()));

					entities[i-2].addPosition(pos);
				}
			}
		}

		ifs.close();
	}

	return true;
}

// Print Report
void Game::printReport()
{
	int infected_count = 0;
	int dead_count = 0;
	int normal_count = 0;
	int recovered_count = 0;

	for (auto& entity : entities) {
		if (entity.isAlive()) {
			if (entity.isInfected()) {
				infected_count++;
			}
			else {
				if (entity.isRecovered())
					recovered_count++;
				else
					normal_count++;
			}
		}
		else {
			dead_count++;
		}
	}

	std::ofstream ofs(output_filename);

	ofs << "Normal     : " << normal_count << std::endl;
	ofs << "Infected   : " << infected_count << std::endl;
	ofs << "Dead       : " << dead_count << std::endl;
	ofs << "Recovered  : " << recovered_count << std::endl;

#ifdef DEBUG
	std::cout << "Normal     : " << normal_count << std::endl;
	std::cout << "Infected   : " << infected_count << std::endl;
	std::cout << "Dead       : " << dead_count << std::endl;
	std::cout << "Recovered  : " << recovered_count << std::endl;
#endif

	for (auto& entity : entities) {
		ofs << "entity " << entity.getId() << " " << entity.getPosition().x << "x" << entity.getPosition().y << " ";
#ifdef DEBUG
		std::cout << "entity " << entity.getId() << " " << entity.getPosition().x << "x" << entity.getPosition().y << " ";
#endif
		if (entity.isAlive()) {
			if (entity.isInfected()) {
				ofs << "infected" << std::endl;
#ifdef DEBUG
				std::cout << "infected" << std::endl;
#endif
			}
			else {
				if (entity.isRecovered()) {
					ofs << "immune" << std::endl;
#ifdef DEBUG
					std::cout << "immune" << std::endl;
#endif
				}
				else {
					ofs << "normal" << std::endl;
#ifdef DEBUG
					std::cout << "normal" << std::endl;
#endif
				}
			}
		}
		else {
			ofs << "dead" << std::endl;
#ifdef DEBUG
			std::cout << "dead" << std::endl;
#endif
		}
	}

	ofs.close();
}


// Print Board
void Game::printTurn(int turn)
{
	std::vector<std::vector<int>> board;

	for (int i = 0; i < this->size + 2; i++) {
		std::vector<int> vec;

		if ((i == 0) || (i == this->size + 1)) {
			for (int j = 0; j < this->size + 2; j++) {
				vec.push_back(-1);							// -1: Wall
			}
		} 
		else {
			vec.push_back(-1);

			for (int j = 1; j <= this->size; j++) {
				vec.push_back(0);							// 0: Empty
			}

			vec.push_back(-1);
		}

		board.push_back(vec);
	}

	for (auto& entity : this->entities) {
		Position& pos = entity.getPosition();

		if (entity.isAlive()) {
			if (entity.isInfected() || entity.isIncubation())
				board[pos.x][pos.y] = 2;					// 2: Infected or Incubated
			else
				board[pos.x][pos.y] = 1;					// 1: Normal
		}
		else {
			if (entity.isInfected())
				board[pos.x][pos.y] = 3;					// 3: Dead, but can infect
			else
				board[pos.x][pos.y] = 0;					// Erase dead from the board after 5 days (when not infected anymore)
		}	
	}

	std::ofstream ofs;

	if (turn == 1)
		ofs.open(turn_filename);
	else
		ofs.open(turn_filename, std::ios_base::app);

	ofs << "Turn " << turn << ":" << std::endl;

#ifdef DEBUG
	std::cout << "Turn " << turn << ":" << std::endl;
#endif

	for (int i = 0; i < this->size + 2; i++) {

		for (int j = 0; j < this->size + 2; j++) {
			switch (board[i][j]) {
				case 3:
					ofs << "D";
#ifdef DEBUG
					std::cout << "D";
#endif
					break;
				case 2:
					ofs << "X";
#ifdef DEBUG
					std::cout << "X";
#endif
					break;
				case 1:
					ofs << "O";
#ifdef DEBUG
					std::cout << "O";
#endif
					break;
				case 0:
					ofs << " ";
#ifdef DEBUG
					std::cout << " ";
#endif
					break;
				case -1:
					ofs << "-";
#ifdef DEBUG
					std::cout << "-";
#endif
					break;
			}
		}

		ofs << std::endl;
#ifdef DEBUG
		std::cout << std::endl;
#endif
	}
}

void Game::run()
{
	for (int day = 1; day <= this->turn_count; day++) {
		for (auto& entity : this->entities) {
			// Update status
			if (entity.isInfected()) {
				entity.increaseInfectedDay();

				if (entity.getImmuneStatus() == DIE) {
					// Entities which will be died, are died after 14 days but can infect other entities
					// After 14 + 5 days, they cannot infect any other entities any more
					if (entity.getInfectedDay() == (14 + 1)) {
						entity.setAlive(false);
					}
					else if (entity.getInfectedDay() == (14 + 5 + 1)) {
						entity.setInfected(false);
						entity.resetInfectedDay();
					}
				}

				// After 30 days, entities which can be recovered are recovered
				if (entity.getImmuneStatus() == RECOVER && entity.getInfectedDay() == (30 + 1)) {
					entity.setRecovered(true);
					entity.setInfected(false);
					entity.resetInfectedDay();
				}
			}
			else if (entity.isIncubation()) {
				entity.increaseIncubationDay();
				entity.increaseInfectedDay();

				// After 3 days, entities in incubation become infected
				if (entity.getIncubationDay() > 3) {
					entity.setInfected(true);
					entity.setIncubation(false);
					entity.resetIncubationDay();
				}
			}
		}

		// Move all entities to their new positions
		for (auto& entity : this->entities) {
			entity.move(day);
		}

		printTurn(day);

		// Infect entities which can be infected in the new positions
		for (auto& entity : entities) {
			for (auto& e : entities) {
				if (entity.getId() != e.getId() && entity.canInfect(e)) {
					e.setIncubation(true);
				}
			}
		}
	}
}

// Main
int main(int argc, char *argv[])
{
	Game game;

	game.init("INPUT.TXT", "OUTPUT.TXT", "TURNS.TXT");

	game.run();
	game.printReport();

	

	return 0;
}
