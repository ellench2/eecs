"""
EECS 445 - Introduction to Machine Learning
Winter 2023 - Homework 3
Spectral Clustering
"""
import matplotlib.pyplot as plt
import numpy as np
from scipy.linalg import eigh


def spectral_clustering():
    '''
    As specified in Question 4, you will initialize the laplacian computed in part (a), compute the eigenvectors corresponding
    to the k lowest eigenvalues, and plot the rows of the eigenvectors to determine the cluster assignments.
    '''
    L = [[1.4167, -0.75, -0.333, 0, -0.333],
        [-0.75, 1.5, -0.25, -0.5, 0],
        [-0.333, -0.25, 1.58333, -0.8, -0.2],
        [0, -0.5, -0.8, 1.3, 0],
        [-0.333, 0, -0.2, 0, 0.5333]]
    evals, evecs = eigh(L, subset_by_index=[0,2])
    print(evals.round(4))
    print(evecs.round(4))

if __name__ == '__main__':
    spectral_clustering()