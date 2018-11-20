""" Some helper functions """

from pathlib import Path
from subprocess import check_output


def contains(col, item):
    """ Returns true if the collection col contains the item item. """
    return len(list(filter(lambda x: x.find(item) >= 0, col))) >= 1


def docker_image_exists(name):
    """ Returns true if a Docker image named name exists. """
    output = check_output(["docker", "images"]).decode("utf-8")
    return contains(output.splitlines(), name)


def docker_container_exists(name):
    """ Returns true if a Docker container named name exists. """
    output = check_output(["docker", "ps", "-a"]).decode("utf-8")
    return contains(output.splitlines(), name)


def fix_wsl_path(path):
    """ Replaces /mnt/... by /host_mnt/... in paths starting with /mnt/ """
    # workaround for the problem described here:
    # https://forums.docker.com/t/volume-mounts-in-windows-does-not-work/10693/169
    parts = list(Path(path).parts)
    parts[1] = "host_mnt" if len(parts) > 1 and parts[1] == "mnt" else parts[1]
    return Path(*parts)


def resolve_path(path):
    """ Replacement for Path.resolve() which does not seem to
        work properly on WSL """
    if not path.is_absolute():
        return Path.cwd() / path

    return path
