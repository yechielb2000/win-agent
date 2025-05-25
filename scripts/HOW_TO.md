## How to run the service.

```shell
sc create WinAgent binPath= "path\to\WinAgent.exe"
```

Make sure an output.csv file is in `C:\output.csv` (yes. disgusting... sorry)

To run the service run.

```shell
sc start WinAgent
```

Stop with

```shell
sc stop WinAgent
```

