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


***Inline***:  
`$E = mc^2$` 

**Resultaat:**

$E = mc^2$ 

***Blok***:

```markdown
De formule is: 
$$
z = \frac{\Delta p}{\rho g}
$$
```

**Resultaat:** 

De formule is:
$$
z = \frac{\Delta p}{\rho g}
$$

ℹ️ ️**Niet** alle Markdown-omgevingen ondersteunen formules.

> Tip: speciale tekens weergeven

Markdown gebruikt tekens als #, *, _, `.
Als je die letterlijk wilt laten zien, zijn er twee mogelijkheden:

Zet ze in een code block of inline code:

Gebruik `#` voor een titel.

Gebruik een backslash-escape:

\*niet cursief\*


# 🛠️ Tools & editors voor Markdown
🔹 1. **Simpele teksteditors** (met preview)

  - VS Code (gratis, Windows/Mac/Linux)
 heel populair, ingebouwde Markdown preview *(Ctrl+Shift+V)*.

  - Atom (open source, GitHub)

  - Sublime Text (lichtgewicht, snel, cross-platform)

🔹 2. **Online editors**

  - StackEdit (https://stackedit.io
) → online editor, direct preview.

  - Dillinger (https://dillinger.io
) → drag & drop bestanden, export naar PDF/HTML.

  - HackMD / CodiMD → samenwerken aan Markdown-documenten (vergelijkbaar met Google Docs).

🔹 3. **Notitie-apps**

  - Obsidian (Markdown als database voor notities, heel krachtig voor kennismanagement).

  - Typora (minimalistische editor, what you see is what you get voor Markdown).

  - Joplin (open-source notitie-app, synchronisatie mogelijk, gebaseerd op Markdown).

  - Notion (niet puur Markdown, maar ondersteunt veel Markdown-syntax).

🔹 4. **Voor programmeurs en data analyse**

  - Jupyter Notebooks / JupyterLite →
 *Perfect voor uitleg en documentatie naast code.*

  - GitHub / GitLab / Bitbucket →
 *Markdown* is dé standaard voor documentatie (README.md).

  - RStudio → vooral in data science, met RMarkdown (uitbreiding voor rapporten).

🔹 5. **Converteren en exporteren**

  - Pandoc (command-line tool om Markdown om te zetten naar PDF, Word, HTML, enz.)

  - Mark Text (lichte open-source editor met exportmogelijkheden).