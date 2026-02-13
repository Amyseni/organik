# How do I get started?
This will always be [directly on the latest release.](https://github.com/Amyseni/organik/releases/latest) Need help/have a specific issue? [Discord is the place to be](https://discord.gg/NX9bSSEq9v)

## ORGANIK is free and open source
And always will be. In my opinion, open sourcing even the things you struggle to be proud of is the only way to truly reveal the space that would have been left `void` by your silence.
Basically, it's got some quirks but ORGANIK is really cool.

# Credits
So many people to thank... I really hope I got everyone, but **please** if your name is missing, reach out and let me know. 

Money Givers
=
### Mysterious Benefactor
@inspectorboat - Direct Donation / Other. Project would be dead in the water if not for Boat encouraging me/pushing me to figure things out and letting me talk her ear off at all hours of the damn day. And then still gave more on top of that. 

### "Big Money, Big Prizes"
@suny_n - Power Grid Supervisor / "Let's play games" -> waits for me to start the call (true combo) - Direct Donation / PayPal
@simonsis - "Big Tech"/Electrical Expert/Analyst ~~& secret backroom lover~~ (close friend, the definitive GOAT breacher, supported me since long before day 1) - Patreon

### Venture Capital / Tooltip Ignorer (Patreon)
TODO: find another @simonis

### Raider Enjoyer (Patreon)
TODO: write a snappy one-liner

Other Projects / Libraries / Art & Audio / Special Thanks
==
*(any code included in any mod version, past or present--including code indirectly based on existing projects. Art, sfx, music--always used with permission. Active/involved community members. Anyone else Organik couldn't exist without). In no particular order.*

### People
  - [Kilburn (@FixItVinh)](https://x.com/fixitvinh) (Creator of ZHL/[Antibirth](https://www.antibirth.com/)) - Genius/Brain person, inspired me to be more than I thought possible, and who without meaning to, taught me that doubting your own work is just another thing made easier with greater skill.
  - [Archie](https://github.com/Archie-osu) (Creator of [Aurie](https://github.com/AurieFramework/Aurie) / [YYToolkit](https://github.com/AurieFramework/YYToolkit), oracle of gamemaker secrets/cool person extraordinaire) -- Saved me from an unknowable length of time reverse engineering an entire GameMaker.<br>
  - @en.ge ( [ART (cara.app) ](https://cara.app/enge) ) - Banner art (Patreon, Youtube), Concept art, (actual, serious) idea creator/organizer/filter and backboard, inspirer of the improbable. Did I mention art? Also an artist. A really good one. Like dam.<br>
  - NotYourSagittarius ([tweeter](https://x.com/NY_Sagittarius )) - The Robo-Mushroom profile-pic/icon art (MushBot). Also just a very cool person (insanely talented)<br>
  - @simonsis (AGAIN??) - The only reason I got good enough at Synthetik to care about modding it.<br>
  - @tactu - Keeping my head on straight. Maybe the person outside myself who cares most deeply about this mod being done "right," and not just "done."<br>
  - @omnx - ADHD Squirrel/DistractionFactory::Create()/"I bet you could train an LLM to do this" incarnate. Also saved me countless hours of spinning in circles/weeks of potential dev time<br>

### Projects
  - Aurie/YYToolkit (links above) - The rock solid foundation upon which I constructed this rickety 100ft ladder to nowhere. The GOAT libraries that got me started understanding GameMaker/YYC to where I am now (aka--still clueless). The OG source for all gamemaker structs/types/the YYC binary format used in all my code here. (This saved at least a year of dev time, probably a lot more).<br>
  - [ImGui](https://github.com/ocornut) -- Does what it says on the tin. I don't even want to think about the reality where I kept on with my own GUI "solution" (read: problem)<br>
  - [Tiny Mod Injector](https://github.com/YAL-Game-Tools/TinyModInjector)<br> -- Freed me from the peril of trying to support a launcher .exe / installer package for both Proton AND Windows itself. (effectively saving me from writing TinyModInjector)
  - [CallbackManagerMod - hooking/callbacks library.](https://github.com/PippleCultist/CallbackManagerMod) -- **The** "it just works" callbacks/hooking package for YYToolkit and GameMaker games. Seriously, it just does the thing and that is magical all by itself.<br>
      - (and other [PippleCultist](https://github.com/PippleCultist) works (incl. HolocureMultiplayerMod): inspiration and code references.)<br>
  - LibZHL/LibZHLGen (now Arkitekt) - The "Magic Sauce" that binds and hooks functions and variables using to make everything else about the mod work. Without this, Organik would not be. <br>
     - Created by [Kilburn (@FixItVinh)](https://x.com/FixItVinh/) (see above) for his mod Antibirth (later Repentance). Modified briefly by myself and TheSwiftTiger, then by the [FTL Hyperspace](https://github.com/FTL-Hyperspace/FTL-Hyperspace) team, then back to me as I rewrote the whole thing using LuaJIT's DynASM to dynamically emit hook code at runtime.<br>
  - DynASM - as mentioned above, Arkitekt now uses DynASM (but not the rest of LuaJIT) to dynamically generate x86 assembly for hooking functions at runtime. It's insane, it's stupid, no one should ever try it. It's really fast.<br>
     - Written by the [LuaJIT](https://luajit.org) team. Licensed under MIT.<br>
    
# Licensing
[Organik - GPL3](https://github.com/Amyseni/organik/blob/master/LICENSE)<br>
[LibZHL,LibZHLGen,Direktor,Komposer - MIT](https://github.com/Amyseni/organik/blob/AmyDev/LICENSE-LibZHL) -- **Any future use** must be attributed to [Kilburn (@FixItVinh)](https://x.com/FixItVinh/) -- See above. 'Nuff said. <br>
[YYToolKit - AGPL3](https://github.com/AurieFramework/YYToolkit/blob/stable/LICENSE)/[Aurie Framework - AGPL3](https://github.com/AurieFramework/Aurie/blob/master/LICENSE)<br>
[Tiny Mod Injector - MIT](https://github.com/YAL-Game-Tools/TinyModInjector/blob/main/LICENSE)<br>
[CallbackManagerMod - No licensing info provided](https://github.com/PippleCultist/CallbackManagerMod)<br>
[ImGui - MIT](https://github.com/ocornut/imgui/blob/master/LICENSE.txt)<br>
[DynASM - MIT](https://luajit.org/dynasm.html)

## Take My Money! (Donations/Support)
If you insist, I have money pits provided for you:<br>
[Here (Patreon) - If you only hate money](https://www.patreon.com/amyseni/membership) <br>
[Here (PayPal) - If you hate money AND Patreon](https://paypal.me/OrganikMod?country.x=US&locale.x=en_US)<br>
_DeFi wallet(s)/other donation methods available on special request/just come find me on discord @amyseni_ <br><br>
**Donations should always be viewed as precisely that, donations.** While there are benefits/rewards associated with your generosity (on Patreon always, but available on request to donations outside Patreon, subject to my ability to effectively distrbiute/manage--so don't count on it). <br><br>
**These are not products available for purchase**, merely a representation of my thanks for your support. In the event of a sudden influx of new members, please understand that there may be unforeseen issues/delays in making these available to everyone. In the event this happens, please reach out to me on Discord (link below, @amyseni) and I will do my best to find a solution that will make things right. Reward tiers may change (with advance notice via Discord) <br><br>
Truly, **the best way you can support** me/the project is by **sharing with anyone you know** who might be interested. For Synthetik particularly, we are especially fighting against the fact that many people have given up hope that the game's many issues can/will ever be fixed. If word gets out there far enough, they will come running.<br>

### Social Media / Links
[Patreon - Main Page / Feed](https://www.patreon.com/amyseni)<br>
[Join Us on Discord - Official Organik Hangout](https://discord.gg/KrTA2QkyCx)<br>
[YouTube - More coming soon here](https://www.youtube.com/@OrganikMod)
