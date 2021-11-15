# C++ Tutorial
***
### Author Keisuke Noji @CVSLab.
### Tuesday, November, 16, 2021  
***
## Processing Flow
|Function|contents|
|:------:|:------|
|generate image|create sample folder<br> create noise image|
|trimming|input: folder<br> set trimming coordinate<br> Trim and plot graph|
***
## set Params (params.h)
|Functional|default|Contents|
|:-----:|:-----|:-----|
|sample_dir|'sample'|sample image generate path|
|extension|'.bmp'|generate and read image extension|
|width|256|generate and read image size width|
|height|256|generate and read image size height|
|number|10| generate number|
|threshold|100|brightness value threshold|