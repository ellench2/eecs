"""
EECS 445 - Introduction to Machine Learning
Winter 2023 - Homework 3
PCA
"""
import os
from pathlib import Path

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
from PIL import Image


def pca(data):
    """Run PCA on a centered matrix `data`.

    `data` is the (N, D) shaped X matrix from problem 5.1.

    Returns: tuple of (eigenvalues, principle_axes), sorted by
    eigenvalues from highest to lowest
    
    eigenvalues is (D,) shaped vector of eigenvalues
    principle_axes is a (D, D) shaped matrix where the ith row is the
        eigenvector that corresponds to the ith largest eigenvalue
    """
    # First, let's find the principle components
    # As you derived, the nth principle component is the nth eigenvector

    # TODO: matrix = ??? (the matrix of which we want the eigenvectors)
    # TODO: eigenvals, eigenvecs = ???
    matrix = (1/(np.shape(data)[0]))*np.matmul(np.transpose(data),data)
    eigenvalues, eigenvectors = np.linalg.eig(matrix)
    return eigenvalues,eigenvectors
    # TODO: return (???, ???)


def plot(data, pca):
    """Plot PCA output.

    `data` is the centered data matrix
    `pca` is the output returned by calling the pca function
    """
    ax = plt.axes(projection="3d")
    ax.scatter3D(data[:, 0], data[:, 1], data[:, 2], s=4)
    ax.set_xlabel('PC 1')
    ax.set_ylabel('PC 2')
    ax.set_zlabel('PC 3')
    vecs = []
    ev = pca[0] / pca[0].sum()
    for var, vec in zip(ev, pca[1]):
        vecs.append(4 * vec * np.sqrt(var))
    pc = np.stack(vecs).T
    ax.quiver(
        np.zeros(len(vecs)),
        np.zeros(len(vecs)),
        np.zeros(len(vecs)),
        pc[0],
        pc[1],
        pc[2],
        color="red",
    )
    plt.savefig("./pca_output.jpg")


def generate_data():
    """Generate dataset."""
    rng = np.random.RandomState(21)
    data = np.dot(rng.rand(3, 3), rng.randn(3, 300)).T
    # center the data so it has mean of zero
    data -= data.mean(axis=0)
    return data


def read_image(path):
    im = Image.open(path)
    im = im.resize((64, 64))
    im = np.asarray(im)
    im = im.mean(axis=2)
    return im


def get_images():
    images = []
    data_dir = Path("./faces")
    paths = list(os.listdir(data_dir))
    for path in paths:
        images.append(read_image(data_dir / path))
    pathdict = {path: i for i, path in enumerate(paths)}
    return np.array(images), pathdict


def get_image_from_filename(filename, images, pathdict):
    ind = pathdict[filename]
    return images[ind]


def get_nn(vec, images, k=6):
    # cosine similarities
    sim = np.matmul(images, vec) / (
        np.linalg.norm(images, axis=1) * np.linalg.norm(vec)
    )
    topk = sim.argsort()[-k:][::-1]
    return topk, images[topk], sim[topk]
