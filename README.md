# NML

NML (Node Markup Language) is a XML-Like language capable of storing trees. 

Example of code :
```
// Example file
<NML version=true>	// This is a comment!
	<project name=' '/>	
</NML>
<OPDIV>
	[LEFT]
	<OPADD>
		[LEFT]
		<CONST content=3.14/>
		[RIGHT]
		<CONST content=""/>
	</OPADD>
	[RIGHT]
	<OPMUL>
		[LEFT]
		<CONST content=2/>
		[RIGHT]
		<CONST content=3/>	
	</OPMUL>
</OPDIV>
```


It's simple and clear, the [LEFT] and [RIGHT] tags are used to show if a node is on the left or right part. You can make your own "Node" class that uses other type of tags without any problems ! As long as your data can be stored in this format, you can use my interpreter!

Once your tree has been generated, you can navigate any node, add attributes and change the current attributes then re-compile the tree!

Goal of this project :

Learn more about parsing techniques and compilation theory in general.
