#include <iostream>

#define mapHeight 5 ///Height of the map
#define mapWidth 5 ///Width of the map
#define maxItemsInRoom 2 ///How many items there can be lying on the floor in one room at the same time.
#define inventorySpace 2 ///How much room the player has in their inventory

using namespace std;

///DISCAILMER: Don't do everything exactly as I do here, this is just an example game in order to introduce  you to more advanced techniques. Think by yourself.

enum ErrorList{ ///Enums are basically a list of static integer variables. Unless assigned otherwise, each next variable is 1 more than the last.
    ERROR_NONE,
    ERROR_INVALID_INPUT,
    ERROR_NOT_PASSABLE,
    ERROR_NOT_A_DOOR,
};

enum TileList{
    TILE_EMPTY,
    TILE_WALL,
    TILE_CLOSED_DOOR,
    TILE_OPEN_DOOR,
};

struct TILE_TYPE{ ///This is a struct, it's used as a sort of;
	bool isPassable;
};

void clearScreen(); ///Prototype for the clearScreen() function
void displayText(string input); ///Prototype for the displayText() function
string processInput(string input); ///Prototype for the processInput() function. String instead of void means this function returns a string.
bool isPassable(int posX, int posY); ///Prototype
bool insideMap(int posX, int posY); ///Prototype

string mapRoomDescription[mapWidth][mapHeight]; ///Declaring the description array. This is called a 2D array. This is where the default description of the rooms are located. First square brackets is the X coordinate, second is the Y coordinate.
/** A visiual representation is something like this
Y0 X0{"Red room"} X1{"Gray room"} X2{"Dark room"}
Y1 X0{"Bloody room"} X1{"There is a window here, it lets a small amount of light through it, you can't make out anything through the window"} X2{"Example room"}
Y2 X0{"Seventh room"} X1{"Wall to the south"} X2{"Corner in the south east"}
The total amount of rooms, as you could probably count, is 9(mapHeight*mapWidth=3*3=9). This is just an example to visualize the description array, this is not the actual contents**/

/**This map array is of type 'TileList', Which means it's restricted to the values in the TileList enum.
Instead of using the TileList type you can simply use int. Then you have the ability to use integers and might be easier to see, the negative is that there is less clear on what is what and the program won't call you out on tiles that don't exist.
but if you are working alone this might be preferred.
This is the *actual* map array, where the different elements are located. Stuff like doors, and walls, etc. **/
TileList mapArray[mapWidth][mapHeight] = {
    {TILE_WALL,  TILE_WALL,  TILE_WALL,          TILE_WALL,  TILE_WALL}, /// (1,1,1,1,1)
    {TILE_WALL,  TILE_EMPTY, TILE_CLOSED_DOOR,   TILE_EMPTY, TILE_WALL}, /// (1,0,2,0,1)
    {TILE_WALL,  TILE_EMPTY, TILE_WALL,          TILE_WALL,  TILE_WALL}, /// (1,0,1,1,1)
    {TILE_WALL,  TILE_EMPTY, TILE_EMPTY,         TILE_EMPTY, TILE_WALL}, /// (1,0,0,0,1)
    {TILE_WALL,  TILE_WALL,  TILE_WALL,          TILE_WALL,  TILE_WALL}, /// (1,1,1,1,1)
};
int itemArray[mapWidth][mapHeight][maxItemsInRoom]; ///X and Y coordinate array for items lying in a room. First square brackets are X coordinate, second is Y, third are the spots in where the items can lie on the ground.

int playerInventory[inventorySpace]; ///How much space in the players inventory
int playerRoomPos[2] = {1, 1}; ///playerRoomPos[0] is the X coordinate, playerRoomPos[1] is the Y coordinate. See the connection with the map array? This is importiant. Alternatively you can create 2 variables, playerX and playerY. It might be easier to see, but then you create multiple variables.
int playerLookPos[2] = {-1, -1}; ///Sort of redundant, not clean code. But it works.
ErrorList invalidInput = ERROR_NONE;

TILE_TYPE tileIndex[] = { ///Initialization of the struct
	{true}, // (0) TILE_EMPTY
	{false}, // (1) TILE_WALL
	{false}, // (2) TILE_CLOSED_DOOR
    {true}, // (3) TILE_OPEN_DOOR
};

int main(){
    bool done = false; ///Declaration of the variable to check whether or not the game is 'done' (exiting the game/winning/losing)
    string playerInput = ""; ///Declaration of the variable to get the players input

    clearScreen(); ///Calling the clearScreen() function. Doing this at the start so that the screen stays consistent.

    ///Manually declare the descriptions of the rooms, no real better way to doing this, as you can't just randomly generate this. Or well, you can, but it won't be as good.
    mapRoomDescription[1][1] = "You are in the top-left corner of the map";
    mapRoomDescription[1][2] = "You are in the center-left of the map";
    mapRoomDescription[1][3] = "You are in the bottom-left corner of the map";
    mapRoomDescription[2][1] = "You are in the top-center of the map. At the door";
    ///mapRoomDescription[2][2] = "You are in the center of the map"; There is a wall here. Nothing to write.
    mapRoomDescription[2][3] = "You are in the bottom-center of the map";
    mapRoomDescription[3][1] = "You are in the top-right corner of the map";
    ///mapRoomDescription[3][2] = "You are in the center-right of the map"; Wall here too.
    mapRoomDescription[3][3] = "You are in the bottom-right corner of the map";

    while(!done){ ///Game loop
        displayText(playerInput); ///Calling the displayText() function
        getline(cin, playerInput); ///Getting the player input
        playerInput = processInput(playerInput); ///Calling the processInput() function. PlayerInput is equal to the function because it returns a string. This is the updated playerInput.
        clearScreen(); ///Calling the clearScreen() function
    }
}

void clearScreen(){
    cout << string(25, '\n');
}

void displayText(string input){ ///Implementation of the displayText() function
    cout << mapRoomDescription[playerRoomPos[0]][playerRoomPos[1]] << "\n\n"; ///\n does essentially the same thing as endl, but it is a bit faster than endl.
    /** Okay, this is a bit tricky.
    What happens here is that you write out the description from the mapRoomDescription array from the playerRoomPos[0] in the first square brackets and playerRoomPos[1] in the second square brackets. And then ending the line.
    As noted in the comments beside the declaration of the coordinate arrays, the first coordinate is X and the second is Y, which is true for both the player coordinates and the map coordinates.
    So we use playerRoomPos[0] as the X coordinate and playerRoomPos[1] as the Y coordinate to access the room description from the same X and Y coordinate as the player is located in.
    Do note that this can be done differently, you are not confined to doing things in the same order or in the same way. It is just much easier to follow and remember, it's also consistent which is important to have clean code.
    This will write the contents of that position in the array, **/

    if(input.size() > 0 && invalidInput == 0){ ///Check if the player has entered something
        cout << "You entered: " << input;
    }else if(invalidInput == ERROR_INVALID_INPUT){
        cout << "Invalid input. Please make sure you typed the command in correctly";
    }else if(invalidInput == ERROR_NOT_PASSABLE){
        cout << "There is something in the way, look in the same direction for more info";
    }
    if(input.size() > 0)
        cout << string(2, '\n');

    if(insideMap(playerLookPos[0], playerLookPos[1])){ ///The check to see if the player actually looked somewhere
        switch(mapArray[playerLookPos[0]][playerLookPos[1]]){ ///A switch. Useful for telling what to do if a variable is equal to different values.
            case TILE_EMPTY:
                cout << "There doesn't appear to be anything special there.";
                break; ///Important to break

            case TILE_WALL:
                cout << "There is a flat wall there.";
                break;

            case TILE_CLOSED_DOOR:
                cout << "There is a closed door there, maybe try opening it?";
                break;

            case TILE_OPEN_DOOR:
                cout << "There is an open door there. I am pretty sure you can go through it now.";
                break;
        }
        cout << string(2, '\n');
        playerLookPos[0] = -1; playerLookPos[1] = -1; ///Sets -1 as an 0 value for the look. Because you can't look outside of the map, as (0, 0) is inside the map.
    }else if(playerLookPos[0] != -1 && playerLookPos[1] != -1){ ///If it isn't the 0 value while the look values are outside the map.
        cout << "There is a flat wall there.";
        playerLookPos[0] = -1; playerLookPos[1] = -1; ///Sets -1 as an 0 value for the look. Because you can't look outside of the map, as (0, 0) is inside the map.
    }
}

string processInput(string input){
    string additionalInput;
    invalidInput = ERROR_NONE;
    /** Moving around, when the player says go (direction) the game checks if the player is allowed to go there. Basically, the game checks if the desired destination is inside the map.
    If we didn't do this we would get out-of-bounds errors which often lead to crashes and weird interactions. **/
    if(input == "go" || input == "move" || input == "travel" || input == "walk" || input == "run"){
        cout << "\nSpecify a direction\n\n";
        getline(cin, additionalInput);
        input.push_back(' '); input.append(additionalInput); ///This is for adding the new input to the string, so that when the game says what you inputted it doesn't leave anyhing out. In addition to a space.
        if(additionalInput == "north" || additionalInput == "up"){
            if(insideMap(playerRoomPos[0], playerRoomPos[1]-1)){
                if(isPassable(playerRoomPos[0], playerRoomPos[1]-1)){
                    playerRoomPos[1]--; ///Removing 1 from the Y coordinate of the player
                }else{
                    invalidInput = ERROR_NOT_PASSABLE;
                }
            }else{
                invalidInput = ERROR_NOT_PASSABLE;
            }
        }else if(additionalInput == "south" || additionalInput == "down"){
            if(insideMap(playerRoomPos[0], playerRoomPos[1]+1)){
                if(isPassable(playerRoomPos[0], playerRoomPos[1]+1)){
                    playerRoomPos[1]++; ///Adding 1 to the Y coordinate of the player
                }else{
                    invalidInput = ERROR_NOT_PASSABLE;
                }
            }else{
                invalidInput = ERROR_NOT_PASSABLE;
            }
        }else if(additionalInput == "west" || additionalInput == "left"){
            if(insideMap(playerRoomPos[0]-1, playerRoomPos[1])){
                if(isPassable(playerRoomPos[0]-1, playerRoomPos[1])){
                    playerRoomPos[0]--; ///Removing 1 from the X coordinate of the player
                }else{
                    invalidInput = ERROR_NOT_PASSABLE;
                }
            }else{
                invalidInput = ERROR_NOT_PASSABLE;
            }
        }else if(additionalInput == "east" || additionalInput == "right"){
            if(insideMap(playerRoomPos[0]+1, playerRoomPos[1])){
                if(isPassable(playerRoomPos[0]+1, playerRoomPos[1])){
                    playerRoomPos[0]++; ///Adding 1 to the X coordinate of the player
                }else{
                    invalidInput = ERROR_NOT_PASSABLE;
                }
            }else{
                invalidInput = ERROR_NOT_PASSABLE;
            }
        }else{
            invalidInput = ERROR_INVALID_INPUT; ///Telling the player that had to type something wrong.
        }
    }else if(input == "look" || input == "see" || input == "peek"){
        playerLookPos[0] = -1; playerLookPos[1] = -1; ///Sets -1 as an 0 value for the look. Because you can't look outside of the map, as (0, 0) is inside the map.
        cout << "\nSpecify a direction\n\n";
        getline(cin, additionalInput);
        input.push_back(' '); input.append(additionalInput); ///This is for adding the new input to the string, so that when the game says what you inputted it doesn't leave anyhing out. In addition to a space.
        if(additionalInput == "north" || additionalInput == "up"){
            playerLookPos[0] = playerRoomPos[0];
            playerLookPos[1] = playerRoomPos[1]-1;

        }else if(additionalInput == "south" || additionalInput == "down"){
            playerLookPos[0] = playerRoomPos[0];
            playerLookPos[1] = playerRoomPos[1]+1;

        }else if(additionalInput == "west" || additionalInput == "left"){
            playerLookPos[0] = playerRoomPos[0]-1;
            playerLookPos[1] = playerRoomPos[1];

        }else if(additionalInput == "east" || additionalInput == "right"){
            playerLookPos[0] = playerRoomPos[0]+1;
            playerLookPos[1] = playerRoomPos[1];
        }else{
            invalidInput = ERROR_INVALID_INPUT;
        }
    }if(input == "open" || input == "unclose"){
        cout << "\nSpecify a direction\n\n";
        getline(cin, additionalInput);
        input.push_back(' '); input.append(additionalInput); ///This is for adding the new input to the string, so that when the game says what you inputted it doesn't leave anyhing out. In addition to a space.
        if(additionalInput == "north" || additionalInput == "up"){
            if(insideMap(playerRoomPos[0], playerRoomPos[1]-1)){
                if(mapArray[playerRoomPos[0]][playerRoomPos[1]-1] == TILE_CLOSED_DOOR){
                    mapArray[playerRoomPos[0]][playerRoomPos[1]-1] = TILE_OPEN_DOOR; ///Removing 1 from the Y coordinate of the player
                }else{
                    invalidInput = ERROR_NOT_A_DOOR;
                }
            }else{
                invalidInput = ERROR_NOT_A_DOOR;
            }
        }else if(additionalInput == "south" || additionalInput == "down"){
            if(insideMap(playerRoomPos[0], playerRoomPos[1]+1)){
                if(mapArray[playerRoomPos[0]][playerRoomPos[1]+1] == TILE_CLOSED_DOOR){
                    mapArray[playerRoomPos[0]][playerRoomPos[1]+1] = TILE_OPEN_DOOR; ///Removing 1 from the Y coordinate of the player
                }else{
                    invalidInput = ERROR_NOT_A_DOOR;
                }
            }else{
                invalidInput = ERROR_NOT_A_DOOR;
            }
        }else if(additionalInput == "west" || additionalInput == "left"){
            if(insideMap(playerRoomPos[0]-1, playerRoomPos[1])){
                if(mapArray[playerRoomPos[0]-1][playerRoomPos[1]] == TILE_CLOSED_DOOR){
                    mapArray[playerRoomPos[0]-1][playerRoomPos[1]] = TILE_OPEN_DOOR; ///Removing 1 from the Y coordinate of the player
                }else{
                    invalidInput = ERROR_NOT_A_DOOR;
                }
            }else{
                invalidInput = ERROR_NOT_A_DOOR;
            }
        }else if(additionalInput == "east" || additionalInput == "right"){
            if(insideMap(playerRoomPos[0]+1, playerRoomPos[1])){
                if(mapArray[playerRoomPos[0]+1][playerRoomPos[1]] == TILE_CLOSED_DOOR){
                    mapArray[playerRoomPos[0]+1][playerRoomPos[1]] = TILE_OPEN_DOOR; ///Removing 1 from the Y coordinate of the player
                }else{
                    invalidInput = ERROR_NOT_A_DOOR;
                }
            }else{
                invalidInput = ERROR_NOT_A_DOOR;
            }
        }else{
            invalidInput = ERROR_INVALID_INPUT; ///Telling the player that had to type something wrong.
        }
    }
    return input;
}

bool isPassable(int posX, int posY){
    if(tileIndex[mapArray[posX][posY]].isPassable){
        return true;
    }
    return false;
}

bool insideMap(int posX, int posY){
    if(posX >= 0 && posY >= 0 && posX < mapWidth && posY < mapHeight){
        return true;
    }
    return false;
}
