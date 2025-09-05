# Korte introductie tot NumPy

NumPy is een Python-bibliotheek die speciaal gemaakt is voor rekenen met grote hoeveelheden data. Het werkt vooral met arrays. Dit zijn eigenlijk lijsten van getallen maar je kunt er *"wiskunde mee doen"* alsof het gewone getallen zijn.

🔹 1. **Importeren**

Eerst importeer je NumPy, meestal zo:

```python 
import numpy as np
```

Hierdoor kun je alle functies van NumPy gebruiken met **np.** ervoor.

🔹 2. **Arrays maken**

Je kunt een array maken van een lijst:

```python 
a = np.array([1, 2, 3, 4])
print(a)         # [1 2 3 4]
print(type(a))   # <class 'numpy.ndarray'>
```

**Arrays** lijken op **Python-lijsten**, maar je kunt er veel **sneller** en **efficiënter** mee rekenen.

🔹 3. **Rekenwerk met arrays**

Met arrays kun je wiskunde direct op alle elementen tegelijk doen. Bekijk deze voorbeelden:
```python 
a = np.array([1, 2, 3, 4])
b = np.array([10, 20, 30, 40])

print(a + b)   # [11 22 33 44]
print(a * 2)   # [2 4 6 8]
print(a ** 2)  # [1 4 9 16]
```

Dus: je hoeft niet te *loopen* over elk element → NumPy doet dat voor je.

🔹 4. **Speciale arrays**

NumPy kan snel "standaard arrays" maken:
```python 
np.zeros(5)       # [0. 0. 0. 0. 0.]   (5 nullen)
np.ones(5)        # [1. 1. 1. 1. 1.]   (5 enen)
# arange gebruik je zo: np.arange(<eerste getal>, <1 meer dan laatste getal>)
np.arange(0, 10)  # [0 1 2 3 4 5 6 7 8 9]
# linspace gebruik je zo: np.linspace(<eerste getal>, <laatste getal>, <aantal getallen>) 
np.linspace(0, 1, 5)  # [0.   0.25 0.5  0.75 1. ]  (5 getallen van 0 tot 1)
```
🔹 5. **Statistiek en functies**

Veel handige functies werken direct op arrays:
```python
data = np.array([1, 2, 3, 4, 5])

print(np.mean(data))   # 3.0 (gemiddelde)
print(np.std(data))    # 1.41 (standaarddeviatie)
print(np.min(data))    # 1
print(np.max(data))    # 5
```

Of wiskundige functies:
```python
x = np.linspace(0, 2*np.pi, 5)
print(np.sin(x))   # sinus van alle waarden tegelijk
```

🔹 6. **Algemeen rekenwerk**

## Het getal **π** 
In NumPy kun je **π** direct gebruiken als **np.pi**.

Voorbeeldjes:
```python
import numpy as np

print(np.pi)           # 3.141592653589793
```

### Een cirkelomtrek berekenen (2πr)
```python
r = 5
omtrek = 2 * np.pi * r
print(omtrek)          # 31.4159...
```
### Een array maken van 0 tot 2π
```python
x = np.linspace(0, 2*np.pi, 5)
print(x)               # [0.         1.57079633 3.14159265 4.71238898 6.28318531]
```
### Sinus van die waarden
```python
print(np.sin(x))       # [ 0.  1.  0. -1.  0.]
```

## De vierkantswortel

De vierkantswortel doe je met **np.sqrt()**.

Voorbeeldjes:
```python
import numpy as np

# Een enkel getal
print(np.sqrt(9))       # 3.0

# Op een hele array tegelijk
a = np.array([1, 4, 9, 16, 25])
print(np.sqrt(a))       # [1. 2. 3. 4. 5.]

# Je kan ook machten gebruiken
print(16 ** 0.5)        # 4.0   (zelfde als sqrt)
print(np.power(16, 0.5)) # 4.0   (NumPy manier)
```

Numpy arrays zijn dus heel handig, je hoeft geen lus te schrijven om bijvoorbeeld wortels of kwadraten van alle waarden in een dataset te nemen.