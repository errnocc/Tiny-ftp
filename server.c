#include "common.h"
#include <unistd.h>
/** 
 * Sets up server and handles incoming connections
 * @param port Server port
 */

int lookup(char *needle, const char **haystack, int count);

void server(int port)
{
  int sock = create_socket(port);
  struct sockaddr_in client_address;
  socklen_t len = sizeof(client_address);
  int connection, pid, bytes_read;

  while(1){
    connection = accept(sock, (struct sockaddr*) &client_address,&len);
    char buffer[BSIZE];
    Command *cmd = malloc(sizeof(Command));
    State *state = malloc(sizeof(State));
    pid = fork();
    
    signal(SIGCHLD,my_wait);
    memset(buffer,0,BSIZE);

    if(pid<0){
      fprintf(stderr, "Cannot create child process.");
      exit(EXIT_FAILURE);
    }

    if(pid==0){
      close(sock);
      char welcome[BSIZE] = "220 ";
      if(strlen(welcome_message)<BSIZE-4){
        strcat(welcome,welcome_message);
      }else{
        strcat(welcome, "Welcome to nice FTP service.");
      }

      /* Write welcome message */
      strcat(welcome,"\n");
      write(connection, welcome,strlen(welcome));

      /* Read commands from client */
      while (bytes_read = read(connection,buffer,BSIZE)){
        

        if(!(bytes_read>BSIZE)){
          /* TODO: output this to log */
          buffer[BSIZE-1] = '\0';
          printf("User %s sent command: %s\n",(state->username==0)?"unknown":state->username,buffer);
          parse_command(buffer,cmd);
          state->connection = connection;
          
          /* Ignore non-ascii char. Ignores telnet command */
          if(buffer[0]<=127 || buffer[0]>=0){
            response(cmd,state);
          }
          memset(buffer,0,BSIZE);
          memset(cmd,0,sizeof(cmd));
        }else{
          /* Read error */
          perror("server:read");
        }
      }
      printf("Client disconnected.\n");
	  free(cmd);
	  free(state);
      exit(0);
    }else{
      printf("closing... :(\n");
      close(connection);
    }
  }
}

/**
 * Creates socket on specified port and starts listening to this socket
 * @param port Listen on this port
 * @return int File descriptor for new socket
 */
int create_socket(int port)
{
  int sock;
  int reuse = 1;

  /* Server addess */
  struct sockaddr_in server_address = (struct sockaddr_in){  
     AF_INET,
     htons(port),
     (struct in_addr){INADDR_ANY}
  };


  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    fprintf(stderr, "Cannot open socket");
    exit(EXIT_FAILURE);
  }

  /* Address can be reused instantly after program exits */
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse);

  /* Bind socket to server address */
  if(bind(sock,(struct sockaddr*) &server_address, sizeof(server_address)) < 0){
    fprintf(stderr, "Cannot bind socket to address");
    exit(EXIT_FAILURE);
  }

  listen(sock,5);
  return sock;
}

/**
 * Accept connection from client
 * @param socket Server listens this
 * @return int File descriptor to accepted connection
 */
int accept_connection(int socket)
{
  socklen_t addrlen = 0;
  struct sockaddr_in client_address;
  addrlen = sizeof(client_address);
  return accept(socket,(struct sockaddr*) &client_address,&addrlen);
}

int lookup_cmd(char *cmd){
  const int cmdlist_count = sizeof(cmdlist_str)/sizeof(char *);
  return lookup(cmd, cmdlist_str, cmdlist_count);
}

int lookup(char *needle, const char **haystack, int count)
{
  int i;
  for(i=0;i<count; i++){
    if(strcmp(needle,haystack[i])==0)return i;
  }
  return -1;
}


/** 
 * Writes current state to client
 */
void write_state(State *state)
{
  write(state->connection, state->message, strlen(state->message));
}

/**
 * Generate random port for passive mode
 * @param state Client state
 */
void gen_port(Port *port)
{
  srand(time(NULL));
  port->p1 = 128 + (rand() % 64);
  port->p2 = rand() % 0xff;

}

/**
 * Parses FTP command string into struct
 * @param cmdstring Command string (from ftp client)
 * @param cmd Command struct
 */
void parse_command(char *cmdstring, Command *cmd)
{
  sscanf(cmdstring,"%s %s",cmd->command,cmd->arg);
}

/**
 * Handles zombies
 * @param signum Signal number
 */
void my_wait(int signum)
{
  int status;
  wait(&status);
}

int main(void)
{
  server(8021);
  return 0;

}
