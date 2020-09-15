#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>

// TODO: automate opponent move, check for winner

typedef struct {
  int size;
  char *board_arr;
} board;

void set_board(board *game, int length){
  game->size = length;
  game->board_arr = (char*) calloc(length * length, sizeof(char));
  for (int a=0; a<(length*length);a++){
    game->board_arr[a] = ' ';
  }
}

void print_board(board *game){
  int total = game->size * game->size;
  char row = 'a';
  char col = '1';
  char row_separator[(game->size*4)];

  for (int a=0; a< (game->size*4); a++){
      row_separator[a] = '-';
  }

  for (int b=0; b<=total; b++){
    if (b % game->size == 0 && b !=0){
      printf(" %c ", row);
      printf("\n%s\n", row_separator);
      row++;
      if (row > 'z'){
          row = 'A';
      }
    }

    if (b == total){break;}
    printf(" %c |", game->board_arr[b]);
  }

  for (int c=0; c<game->size; c++){
    printf(" %c |", col);
        col++;
  }
  printf("\n");

}

bool legal_move(char *attempt, board *board, char mark){
  int position = -1;
  int row = attempt[1] - '1';
  int max_pos = board->size*board->size - 1;

  // verify row
  if (attempt[0] >= 'a' && attempt[0] <= 'z') {
    position = attempt[0] - 'a';}
  else if (attempt[0] >= 'A' && attempt[0] <= 'Z') {
    position = attempt[0] - 'A' + 26;}
  if (position < 0 || position >= board->size) {
    printf("\nOoops - your row selection is invalid. Try again.");
    return false;
  }

  // verify column
  if (row >=0 && row < board->size){
    position = (position *(board->size))+row;
    if (board->board_arr[position] == ' '){
      board->board_arr[position] = mark;
      return true;
    }
    else {printf("\nOoops - that square is occupied. Try again.");}
  }
  else {
    printf("\nOoops - your column selection is invalid. Try again.");
  }
  return false;
}

void random_move (board *board, char mark) {
  bool updated = false;
  int pos;

  while (updated == false){
    pos = ((rand()%board->size)*board->size)+(rand()%board->size);
    if (board->board_arr[pos] == ' '){
      board->board_arr[pos] = mark;
      updated = true;
    }
  }

  return;
}

bool find_winner(board *board, char mark){
  int print, start = board->size-1;
  int add = start;
  int max = (board->size*board->size)-1;
  bool found = true;
  // check rows
  for (int r=0; r<board->size; r++){
    for (int c=0; c<board->size; c++){
      if (board->board_arr[(r*board->size) + c] != mark){
        found = false;
        break;
      }
    }
    if (found == true){return true;}
    found = true;
  }

  // check columns
  for (int c=0; c<board->size; c++){
    for(int r=0; r<board->size; r++){
      if (board->board_arr[(r*board->size) + c] != mark){
        found = false;
        break;
      }
    }
    if (found == true){return true;}
    found = true;
  }

  // check diagonals
  for (int r=0; r<board->size; r++){
    if (board->board_arr[r + (r*board->size)] != mark){
      found = false;
      break;
    }
    if (found == true && r >= board->size){return true;}
    found = true;
  }

  while (start < max){
    if (board->board_arr[start] != mark){
      found = false;
      break;
    }
    start += board->size-1;
  }
  
  if (found == true){return true;}
  return false;
}

int main() {
  // hash_table options = create_table();
  char standard_board, player, opponent, active;
  char move[3];
  int moving;
  bool answer = false;
  bool legal;
  int board_size = 3, tot_moves = 0, max_moves;
  bool winner = false;
  board ticTac;
  board *ttt = &ticTac;

  // get board board size
  printf("Would you like a standard 3x3 Tic-Tac-Toe board (y/n)? ");
  scanf(" %c", &standard_board);
  fflush(stdin);

  while (answer != true){
    switch(standard_board){
      case 'y':
        answer = true;
        printf("Standard board it is!\n");
        break;
      case 'Y':
        answer = true;
        printf("Standard board it is!\n");
        break;
      case 'n':
        while (answer != true){
          printf("Please enter the number of rows/columns for your board: ");
          scanf(" %d", &board_size);
          fflush(stdin);
          if (board_size < 3 || board_size > 52){
            printf("Please enter an integer greater than or equal to 3 and less than or equal to 52.");
            board_size = 0;
            }
          else{
            printf("%dx%d board it is!\n", board_size, board_size);
            answer = true;
          }
          }
        break;
      case 'N':
          while (answer != true){
            printf("\nPlease enter the number of rows/columns for your board: ");
            scanf(" %d", &board_size);
            fflush(stdin);
            if (board_size < 3){
               printf("\nPlease enter an integer greater than or equal to 3. ");
                  board_size = 0;
                }
            else{
              printf("\n%dx%d board it is!\n", board_size, board_size);
              answer = true;
            }
          }
          break;
      default:
        printf("\nOops! Please enter'y' for yes or 'n' for no: ");
        scanf(" %c", &standard_board);
        fflush(stdin);
    }
  }
    
  // create board, reset answer
  set_board(ttt, board_size);
  max_moves = board_size*board_size;
  answer = false;

  // will user be X or O
  printf("\nWould you like to be 'X or 'O' (x/o)? ");
  scanf(" %c", &player);
  fflush(stdin);
  while (answer == false){
    if (player == 'x' || player == 'X'){
      player = 'X';
      opponent = 'O';
      answer = true;
    }
    else if (player == 'o' || player == 'O'){
      player = 'O';
      opponent = 'X';
      answer = true;
    }
    else {
      printf("Please enter X or O: ");
      scanf(" %c", &player);
      fflush(stdin);
    }
  }

  // reset answer, determine who is first
  answer = false;
  printf("\nAlright, player %c, would you like to go first (y/n)? ", player);
  scanf(" %c", &active);
  fflush(stdin);
  while (answer == false){
    switch(active){
      case 'y':
        answer = true;
        active = player;
        printf("\nPlayer 1, get ready!\n");
        break;
      case 'Y':
        answer = true;
        active = player;
        printf("\nPlayer 1, get ready!\n");
        break;
      case 'n':
        answer = true;
        active = opponent;
        printf("\nPlayer 2, get ready!\n");
        break;
      case 'N':
        answer = true;
        active = opponent;
        printf("\nPlayer 2, get ready!\n");
        break;
      default:
        printf("\nOops! Please enter'y' for yes or 'n' for no: ");
        scanf(" %c", &active);
        fflush(stdin);
    }
  }

  // instructions
  printf("When it is your turn, enter the row and the column (i.e. a2).\nLet's play!\n");
  srand(time(NULL));
  
  // gameplay
  while (tot_moves <= max_moves){
    legal = false;
    printf("\n");
    print_board(ttt);
    
    if (active == opponent){
      printf("\nOpponent's move!\n");
      random_move(ttt, active);
      // winner = find_winner(ttt, active);
      if (find_winner(ttt, active) == true){break;}
      printf("\nOpp winner %d\n", winner);
      active = player;
    }

    else {
      printf("Enter your move: ");
      scanf(" %s", move);
      fflush(stdin);
      // // check move and make move
      legal = legal_move(move, ttt, active);
      if (legal == true){
        printf("Successful move!\n");
        if (find_winner(ttt, active) == true){break;}
        printf("\nplayer winner %d\n", winner);
        active = opponent;
      }
      else {active = player;}
    }
    tot_moves++;
  }

  // display board and winner
  printf("\n");
  print_board(ttt);

  if (tot_moves == max_moves){
    printf("\n It's a draw!");
  }
  else {
    if (active == player){
      printf("\nYOU WON!");
    }
    else {printf("\nYou lost. Try again. :(");}
  }

  return 0;
}
