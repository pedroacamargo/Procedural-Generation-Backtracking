<h2>Procedural Generation - Backtracking inspired algorithm</h2>

<samp><p>I developed this algorithm for the LA project of my university, it consists in a procedural map generation algorithm built from scratch.</p></samp>

<p>Basically, this algorithm consists in finding a place to create a room, in 12 positions (it's commented in main.c) and recursively create more rooms.
For example, if the room can't be created in position 1, it will be transposed to the position 2, and recursively the room will try to be generated in
each position, if none of the positions are available, the recursiveness will stop and the map will be done. Furthermore, if the room tries to be
generated outside the main screen, the program won't stop. Besides that, each iteration the rooms are connected.</p>

<p><strong>This is a backtracking inspired algorithm and not a backtracking algorithm</strong> -> The <em> backtracking algorithm </em> works by trying out each possible 
 solution in turn and checking whether it satisfies the problem constraints. 
 If a solution violates a constraint, the algorithm backtracks and tries another option.
 This process continues until a solution is found or all possibilities have been tried.
</p>

<hr>
<h3 align="center"><samp><strong>Example of a small map</strong></samp></h3>
<p align="center"><img src="img/example.png" alt="Example picture" width="500" height="500"> <br> <sub>Picture taken at 23/04/2023</sub></p>
<hr>

<h2>How to install</h2>
<p>Just clone the repo inside any directory in your pc</p>
<pre>$ git clone https://github.com/pedroacamargo/backtracking-study</pre>
<p>Type these commands in the given order for the algorithm work as expected</p>
<pre>$ make clean</pre>
<pre>$ make</pre>
<p>Then you can type <strong><code>./play</code></strong> in the terminal and the ncurses window in terminal will open</p>
<p>To generate the map, just hold the key <em><strong>p</strong></em></p>
<hr>
<h3>Developed by:</h3>
<table>
<tr>
 <th><strong><a href="https://github.com/pedroacamargo">Pedro Augusto Camargo</a></strong></th>
</tr>
<tr>
 <td align="center"><img src="https://avatars.githubusercontent.com/u/98715404?s=400&u=1d27a23534700cfa2fb02fc716db89b023abb54d&v=4" width="150"></td>
</tr>
</table>
