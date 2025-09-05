# Korte introductie tot Markdown

**Markdown** is een *lichte opmaaktaal* om tekst te formatteren. Het wordt veel gebruikt in Jupyter Notebooks, GitHub README’s, documentatie enzovoort. Ook deze pagina's zijn in Markdown gemaakt.

Het idee: je schrijft gewone tekst met een paar eenvoudige tekens (#, *, `) om structuur en opmaak toe te voegen.

## 1. **Koppen maken**

Gebruik # aan het begin van een regel:
```markdown
# Kop niveau 1
## Kop niveau 2
### Kop niveau 3
```

**Resultaat:**

# Kop niveau 1
## Kop niveau 2
### Kop niveau 3

## 2. **Vet en cursief**
```markdown
Vet: **tekst** 

Cursief: *tekst* 

Combinatie: ***tekst*** 
```
**Resultaat:**

Vet: **tekst** 

Cursief: *tekst* 

Combinatie: ***tekst*** 

## 3. **Lijsten**
```markdown
Ongenummerd:

- item 1
- item 2
  - subitem


Genummerd:

1. Eerste
2. Tweede
3. Derde
```
**Resultaat:**

Ongenummerd:

- item 1
- item 2
  - subitem


Genummerd:

1. Eerste
2. Tweede
3. Derde

## 4. **Code**

Voor korte stukjes code gebruik je backticks `:
```markdown
`np.array([1,2,3])`
```

**Resultaat:** 

`np.array([1,2,3])`

Voor meerdere regels code gebruik je code blocks met drie backticks:

<pre> ```python 
import numpy as np 
print("Hallo") 
``` </pre>

**Resultaat:**
```python
import numpy as np
print("Hallo")
```
## 5. **Citaten**

Met > aan het begin van de regel:

```markdown
> Dit is een citaat.

```

**Resultaat:**
> Dit is een citaat.


## 6. **Formules (in Jupyter / GitHub)**

Gebruik **LaTeX-stijl** met `$...$`:
```markdown
De formule is: $z = \frac{\Delta p}{\rho g}$
```

**Resultaat:** 

De formule is: $z = \frac{\Delta p}{\rho g}$

ℹ️ ️**Niet** alle Markdown-omgevingen ondersteunen formules.

> Tip: speciale tekens weergeven

Markdown gebruikt tekens als #, *, _, `.
Als je die letterlijk wilt laten zien, zijn er twee mogelijkheden:

Zet ze in een code block of inline code:

Gebruik `#` voor een titel.

Gebruik een backslash-escape:

\*niet cursief\*


✅ Zo heb je de basis om netjes uitleg en oefeningen in Markdown te schrijven.