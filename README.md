# Filament_dehumidifer
3D Print Filament storage dehumidifier. NodeMCU, Airpumps, Lithium chloride solution, DHT22

## Idea
Keep filament within a small box (40x50x40cm) dry.

## Concept
NodeMCU records the humidity with the DHT22 sensor.
Once the upper limit is reached, a 2N3904 transistor drives.
2 electric serial connected mini air pumps (https://www.pollin.de/p/miniatur-luftpumpe-jqb031-3-v-70-ml-min-330096 , Miniatur-Luftpumpe JQB031, 3 V-, 70 ml/min , EAN: 4049702069713) resulting in a current of ~160mA, so the 2N3904 is able to handle it.
These pumps the air through 2 small hoses on the ground of a glas bottle filled to ~30% with saturated lithium chlorid solution.
Once the lower level specified within the ino is reached,the pumps are switched off.

### Why LiCl
Normal desiccants are boring.

### Regeneration
Once the residue is completely dissolved I will boil the solution and reduce it to half the volume

#### Troubles
I'm still struggeling to keep the solution completely stable because of temperature differences.


#### Side node
LiCl is not very nice to metal

