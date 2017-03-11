# pokégen
Pokégen is a Pokémon Generation Ⅲ Pokémon data structure generator.
It may, for example, be used to generate a pokémon to insert in a emulated
copy of any generation Ⅲ Pokémon game. The default output (a hexdump) has
the offsets configured to match up with where you would put the generated
data to replace your first slot with the generated Pokémon.

## Compilation
To compile, run:

    gcc pokémon.c

## Usage
Detailed usage instructions are available by running with the `--help` argument.

Index numbers are required by several options. Here are lists of what those
correspond to:

| Option             | Index List                                                                                                                                                  |
| ------------------ | ----------------------------------------------------------------------------------------------------------------------------------------------------------- |
| -s, --species      | [bulbapedia:List_of_Pokémon by index number (Generation III)](http://bulbapedia.bulbagarden.net/wiki/List_of_Pok%C3%A9mon_by_index_number_(Generation_III)) |
| -i, --item         | [bulbapedia:List of items by index number (Generation III)](http://bulbapedia.bulbagarden.net/wiki/List_of_items_by_index_number_(Generation_III))          |
| -m, --moves        | [bulbapedia:List of moves](http://bulbapedia.bulbagarden.net/wiki/List_of_moves) (beware of Generation Ⅳ+ moves)                                            |
| -k, --met-location | [bulbapedia:List of locations by index number (Generation III)](http://bulbapedia.bulbagarden.net/wiki/List_of_locations_by_index_number_(Generation_III))  |

## Copyright
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

See `LICENSE` for more information.
