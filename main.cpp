#include <iostream>

#define mapHeight 3 ///Height of the map
#define mapWidth 3 ///Width of the map
#define maxItemsInRoom 2 ///How many items there can be lying on the floor in one room at the same time.
#define inventorySpace 2 ///How much room the player has in their inventory

using namespace std;

enum ErrorList{ ///Enums are essensially a list of static integer variables. Unless assigned otherwise, each next variable is 1 more than the last.
    ERROR_NONE,
    ERROR_INVALID_INPUT,
    ERROR_OUT_OF_BOUNDS,
};

void clearScreen(); ///Prototype for the clearScreen() function
void displayText(string input); ///Prototype for the displayText() function
string processInput(string input); ///Prototype for the processInput() function. String instead of void means this function returns a string.

string mapRoomDescription[mapWidth][mapHeight]; ///Declaring the map array. This is where the default description of the rooms are located. First square brackets is the X coordinate, second is the Y coordinate.
/** A visiual representation is something like this
Y0 X0{"Red room"} X1{"Gray room"} X2{"Dark room"}
Y1 X0{"Bloody room"} X1{"There is a window here, it lets a small amount of light through it, you can't make out anything through the window"} X2{"Example room"}
Y2 X0{"Seventh room"} X1{"Wall to the south"} X2{"Corner in the south east"}
The total amount of rooms, as you could probably count, is 9(mapHeight*mapWidth=3*3=9). This is just an example to visualize the map array, this is not the actual contents**/
int itemArray[mapWidth][mapHeight][maxItemsInRoom]; ///X and Y coordinate array for items lying in a room. First square brackets are X coordinate, second is Y, third are the spots in where the items can lie on the ground.

int playerInventory[inventorySpace]; ///How much space in the players inventory
int playerRoomPos[2]; ///playerRoomPos[0] is the X coordinate, playerRoomPos[1] is the Y coordinate. See the connection with the map array? This is importiant.
int invalidInput = ERROR_NONE;

int main(){
    bool done = false; ///Declaration of the variable to check whether or not the game is 'done' (exiting the game/winning/losing)
    string playerInput = ""; ///Declaration of the variable to get the players input

    clearScreen(); ///Calling the clearScreen() function. Doing this at the start so that the screen stays consistent.

    ///Manually declare the descriptions of the rooms, no real better way to doing this, as you can't just randomly generate this. Or well, you can, but it won't be as good.
    mapRoomDescription[0][0] = "You are in the top-left corner of the map";
    mapRoomDescription[0][1] = "You are in the center-left of the map";
    mapRoomDescription[0][2] = "You are in the bottom-left corner of the map";
    mapRoomDescription[1][0] = "You are in the top-center of the map";
    mapRoomDescription[1][1] = "You are in the center of the map";
    mapRoomDescription[1][2] = "You are in the bottom-center of the map";
    mapRoomDescription[2][0] = "You are in the top-right corner of the map";
    mapRoomDescription[2][1] = "You are in the center-right of the map";
    mapRoomDescription[2][2] = "You are in the bottom-right corner of the map";

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
    cout << mapRoomDescription[playerRoomPos[0]][playerRoomPos[1]] << "\n\n"; ///\n does essensially the same thing as endl, but it is a bit faster than endl.
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
    }else if(invalidInput == ERROR_OUT_OF_BOUNDS){
        cout << "There is a wall there, try another direction (out of bounds)";
    }
    if(input.size() > 0)
        cout << string(2, '\n');
}

string processInput(string input){
    string additionalInput;
    invalidInput = ERROR_NONE;
    /** Moving around, when the player says go (direction) the game checks if the player is allowed to go there. Basically, the game checks if the desired destination is inside the map.
    If we didn't do this we would get out-of-bounds errors which often lead to crashes and weird interactions. **/
    if(input == "go"){
        cout << "\nSpecify a direction\n\n";
        getline(cin, additionalInput);
        input.push_back(' '); input.append(additionalInput); ///This is for adding the new input to the string, so that when the game says what you inputted it doesn't leave anyhing out. In addition to a space.
        if(additionalInput == "north" || additionalInput == "up"){
            if(playerRoomPos[1]-1 >= 0){
                playerRoomPos[1]--; ///Removing 1 from the Y coordinate of the player
            }else{
                invalidInput = ERROR_OUT_OF_BOUNDS; ///Telling the player why this wasn't possible. In this case it's because the desired destination is out of bounds of the map.
            }
        }else if(additionalInput == "south" || additionalInput == "down"){
            if(playerRoomPos[1]+1 < mapHeight){
                playerRoomPos[1]++; ///Adding 1 from the Y coordinate of the player
            }else{
                invalidInput = ERROR_OUT_OF_BOUNDS;
            }
        }else if(additionalInput == "west" || additionalInput == "left"){
            if(playerRoomPos[0]-1 >= 0){
                playerRoomPos[0]--; ///Removing 1 from the X coordinate of the player
            }else{
                invalidInput = ERROR_OUT_OF_BOUNDS;
            }
        }else if(additionalInput == "east" || additionalInput == "right"){
            if(playerRoomPos[0]+1 < mapWidth){
                playerRoomPos[0]++; ///Adding 1 from the X coordinate of the player
            }else{
                invalidInput = ERROR_OUT_OF_BOUNDS;
            }
        }else{
            invalidInput = ERROR_INVALID_INPUT; ///Telling the player that he typed it wrong.
        }
    }else{
        invalidInput = ERROR_INVALID_INPUT;
    }
    return input;
}
