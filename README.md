# Make mean dca distribution

version: 2.0

author: yghuang

date: 29.11.2023

## Patch note

03.04.2024 by yghuang (v2.1):

> Plot:

    1. by default using 6 sigma instead of 5 now

    2. replace data set name `DATASET` with the data set energy like `19.6`

> MeanDcaTool: Now using new version (2.1)

    Acutually no impact on this step (as we do not know the parameters before finishing this step)

> prepare.sh:

    Add a force option to remove folders

29.11.2023 by yghuang (v2.0):

> Using new MeanDcaTool package, new design: mean DCAz/xy as function of RefMult
>
> New plot script.
