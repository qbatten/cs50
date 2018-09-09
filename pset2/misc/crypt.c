#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <crypt.h>
#include <cs50.h>

int
main(void)
{
  char *password;
  /* Read in the user’s password and encrypt it. */
  password = crypt(get_string("Password:"), get_string("\nSalt:"));

  /* Print the results. */
  printf("\nHash: %s\n", password);
  return 0;
}