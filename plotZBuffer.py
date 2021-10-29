import matplotlib.pyplot as plt

def read_data(fname):
   A = []
   with open(fname) as f:
      for line in f:
         A.append([float(num) for num in line.split(',')[:-1]])
   return A

def plot_data(A):
   nx = len(A[0])
   ny = len(A)

   x = list(range(nx))
   y = list(range(ny))
   X = [x for i in range(ny)]
   Y = [[y[i]]*nx for i in range(ny)]
   
   plt.figure(figsize=(8, 6), dpi=80)
   plt.pcolormesh(X, Y, A,cmap="Greys", shading='auto')
   plt.axis('scaled')
   plt.clim(0, 1)
   plt.colorbar(label='Z-Buffer Value', fraction=0.046, pad=0.04)
   plt.show()

fname = input('Input the index (eg. 1, 2, 3, ...) of the Z-Buffer save file you would like to plot: ')
A = read_data('ZBufferSaves/save' + fname + '.txt')
plot_data(A)