# CPU Cache Simulation

<img src="https://github.com/user-attachments/assets/d71c2b7b-0ab6-4075-bdaf-718e34588432" alt="Alt Text" width="590" height="520">

This is a simple simulation of a direct-mapped CPU cache with 8-bit cache lines. It also uses a write-back policy for data storage, which you can see as you step through the fixed instruction set.

Unfortunately, the "simulation" aspect of this project is hard-coded, but the results should match the expected behavior.

## Direct-Mapped Cache
A direct-mapped CPU cache maps memory addresses to specific indexes / memory blocks (AKA cache lines) in the cache. Since our example features 16 bytes of RAM but only 4 bytes of cache storage, we need to find a way to map 4 RAM addresses to each cache line. (The image above shows this mapping.)

We do this by using the modulo operator. For each address in RAM, we simply modulo by the cache size (in this case, 4), which will return a value between 0 and 3. This value represents the index of the mapped cache line.

## Write-Back Policy
## Dirty Flag
