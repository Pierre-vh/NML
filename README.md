# NML

NML, for Node Markup Language is a XML-Like language capable of storing tree made of nodes (c++ classes). 
It is fully extensible, as long as your data can be stored in this format.

For now there is not documentation. If you want me to add one, feel free to ask me.
Contact me : pierre.vanhoutryve@gmail.com 



Example of code :

<NML version="true">				
	<project name="NMLTEST"/>					
</NML>
<OPDIV>	
	[LEFT]
	<OPADD>
		[LEFT]
		<CONST content=true/>
		[RIGHT]
		<CONST content="foo"/>
		
	</OPADD>
	[RIGHT]
	<OPMUL>
		[LEFT]
		<CONST content=2/>
		[RIGHT]
		<CONST content=3/>		
	</OPMUL>
</OPDIV>




It's simple and clear, the [LEFT] and [RIGHT] tags are used to show if a node is on the left or right part. You can make your own "Node" class that uses other type of tags without any problems ! As long as your data can be stored in this format, you can use my interpreter ! It also has a fairly stable error detection system for the most common errors, although I can still run into bugs from time to time !

Once your tree has been generated, you can navigate any node, add attributes and change the current attributes then re-compile the tree !

Algorithms used :

Recursive Descent Lexer
The parser is sort-of homemade. It uses a DFA with a LR-Like parsing tables, but goes from top to bottom. So I guess we can call it a LL(1) parser? It is not the only algorithm used in the parser, because this will only 'reduce' tokens, another algorithm (makeTree) exists to build the tree from your Node class. (This one is home made)

Goal of this project :

Learn more about parsing techniques and compiler theory in general.
