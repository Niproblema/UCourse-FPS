# UCourse-FPS
## Unreal fps game



TODO list: 
 1. Something wrong with projectiles. Projectiles are inaccurate. Also Animation changes rotation. First appeared in 251
 1. Moving forward shooting, actor collides with projectile
 1. TP_Character footing blendspace is bad
 1. Player shadows are invisble
 1. Barrier should only be passable for Player Actor. Also CollisionBox is destroying Projectiles
 1. Gun poking out of capsule
 1. Barrier wall is ugly and does not have collisions for anything. Only player should be allowed through
 1. GuardBehaviour sometimes shoots just 1 time, because enemy key gets reset? 282
 1. FP shoulders are visible from underneath
 1. Crouching
 1. First tile should have a wall, or spawning volume adjusted 
 1. Re add Shooting noise Emitter
 1. Player capsule is still colliding after death
 1. AI does not clear focus
 1. AI passes through barrriers like nothing
 1. Player can get stuck on prop right after entering new Tile
 1. Lagspike when generating new tile. NavMesh rebuild is slow
 1. Old props aren't removed on Tile removal
 1. Dead pawn leaves behind capsule with collisions
 1. PostProcessVolume and LightmassImportanceVolume are not used
 
 
 1. PostProcessVolume on BP_Character is weird. Add Vignette maybe
 1. ReAdjusting AIPerception
 1. Clear out unused classes
 1.Read about static and dynamic collision chanells
 1. http://gameprogrammingpatterns.com/flyweight.html
 1.Android Build