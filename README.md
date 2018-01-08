ICE ENGINE ![alt text](https://d30y9cdsu7xlg0.cloudfront.net/png/84009-200.png "Logo Title Text 1")
###### Illusions Creation Environment
-----------------------------------------
###### Designs, Goals, and Techniques for future and working iterations of our engine
---------------------------------------------------------------------------------------------------------

PURPOSE

###### This engine is designed as an educational and professional experiement into the realm of video game systems design, management systems, rendering API's, performance analysis,  the Windows OS, mathematics, including Linear Algebra and Trigonometry, and the principles and designs inherent to C programming languages.
---------------------------------------------------------------------------------------------------------

##### DESIGN GOALS AND REQUIREMENTS
---------------------------------------------------------------------------------------------------------

###### - Reduce Data Dependency
###### - Keep the Engine running at all times, fast iteration
###### - Keep high performance data small and contiguous to fit in data caches
###### - Multicore Utilization using Job-System of worker threads, one for main game thread, others for systems
###### - Handle-table Object System for fast and reliable indexing and usage of game / system objects
###### - Memory Manager using Pool Allocation with Fragmentation Protection to see allocation patterns / tailer code for optimization
###### - Reduce Excessive Compile-Time Coupling

---------------------------------------------------------------------------------------------------------
CURRENT STANDING

###### Sprint 1 - Setup / Design
- [x] Initial Engine Architecture Design
- [ ] Resource Manager Design / Skeleton / Setup
- [x] Memory Manager Design / Implementation
- [ ] Systems Manager / Managers Base Setup
- [x] Vulkan API Base Foundation Setup
- [x] Object Handle-table Design
- [ ] Object Handle-table Implementation

---------------------------------------------------------------------------------------------------------
#### Authors / Responsibilities Currently
---------------------------------------------------------------------------------------------------------

###### Jordan Humphrey

* ###### Engine Architecture Design
* ###### Rendering API (Vulkan) / Systems
* ###### Object Mngmnt/Handle Design
* ###### UI Design/Implementation

###### John Patek

* ###### Resource Management Systems
* ###### Thread-pool Manager
* ###### Task Schedule and Management
* ###### Memory Systems and Analysis