#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "../include/player.h"

int main() {
    Player p = create_player("TestUser", 100.0);

    assert(strcmp(p.name, "TestUser") == 0);
    assert(p.balance == 100.0);

    printf("✅ Test: Player creation OK\n");
    return 0;
}
