# deity
A Flow Based Programming rendering system built in 2001

Deity was developed from a discussion about the best way to manage complex game systems. The discussion lead to the thought experiment of "kicking the bin". The idea was: If you have a character in a game and they need to interact with an object in the scene then how does that process work from a software architecture perspective. When a character can 'kick a bin' what is the events and actions that occur. Initially we considered the OO approach and quickly found problems like:
- If you kick the bin too hard it may dent, or may detach from its mounting. If we encapsulate that object then we need to provide all the objects capabilities internally that can create these results.
- We might have hundreds of bins in a scene, and we do not want to have something to "manage" them all - they should operate without assistance.
- The physics system must not need to be "aware" of the object. All OO examples of physics at the time required quite complex object structures (most still do). 
And many more...

So then we talked about traditional events and signals, and found problems with those paradigms as well. Finally we came up with "data exchange" which was spawn from "how do real objects exchange information" - energy or matter is passed around. 

The bin object was determined to be a generic object in the world that should be a 'black box'. Other objects and systems should not know anything about it. Objects interact with data streams and the object listens to those streams to determine if it needs to interact. This decouples all the function of the object from the world and makes its management utterly simple.

Deity was built to trial the concept. Surprsingly it worked. And it worked very well, but it was hard to train others in the FBP paradigm. I later discovered FBP by Paul Morrison and reaslised this is an old idea, and it does work well :)
https://jpaulm.github.io/fbp/

Deity's implemenation of FBP has a few slightly different interpretations, but its fundamentally the same as Pauls work. Originally Deity was going to be extended to become more like an OS with a kernel (Zeus) and modules to support it. 

Feel free to use this as you want. Im not sure it even compiles :)

License will be MIT inline with Lua's license scheme. 

Hopefully I will progress this in the future. Will see.
