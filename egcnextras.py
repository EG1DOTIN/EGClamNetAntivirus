"""
egcnextras.py :=> This file is used for some extra command in Windows like auto software Update, download
the latest setup file, Checking version info, scanning files with clamd service. Only used in Windows OS.
make exe file with below command
pyinstaller.exe --onefile --name egcnextras.exe egcnextras.py --icon="E:\All\CODE\PYTHON\pyEGAV\Resources\egav.ico"
"""

from sys import argv
from zipfile import ZipFile
from urllib3 import PoolManager
from platform import architecture
from json import loads as json_loads
from requests import get as requests_get
from gdown import download as gdown_download
from PySide6.QtGui import QIcon
from PySide6.QtWidgets import QMessageBox, QApplication
from os import path as os_path, makedirs as os_make_dirs, \
    fsync as os_fsync, environ as os_environ, system as os_system, startfile as os_startfile

# from subprocess import call as subprocess_call


VERSION_STRING_URL = "https://drive.google.com/uc?export=download&id=1aTGtQry1jXm03L9thdTYQCbcyjiLaBME"
UPDATEX64_DOWNLOAD_LINK = "https://drive.google.com/uc?export=download&id=1NeHpJEoNwPN9B6O0J-ZI7eF2G2B2QDrE&confirm=t"
UPDATEX86_DOWNLOAD_LINK = "https://drive.google.com/uc?export=download&id=1fgyOIp0thaxLkFsRThjvbNEgAHnIYreY&confirm=t"

FILENAME_X64 = "EGCNAV_WINx64_UPDATE.zip"
FILENAME_X86 = "EGCNAV_WINx86_UPDATE.zip"
EXEFILENAME_X64 = "EGCNAV_WINx64_UPDATE.exe"
EXEFILENAME_X86 = "EGCNAV_WINx86_UPDATE.exe"


def get_version_dict():
    """
    this function is used to get version information
    :return: (dict)
    """
    try:
        http = PoolManager()
        resp = http.request('GET', VERSION_STRING_URL).data.decode("utf8")
        dict_value = json_loads(resp)
        return dict_value
        # print(resp.data.decode("utf8"))
    except Exception as e:
        print(e)
        return {}


def downloadFile(url: str, destination_folder: str):
    """
    this function is used to download a file from a url
    :param url: (str)
    :param destination_folder: (str)
    :return: (None)
    """
    if not os_path.exists(destination_folder):
        os_make_dirs(destination_folder)  # create folder if it does not exist

    filename = destination_folder + "\\" + FILENAME_X64  # be careful with file names
    file_path = os_path.join(destination_folder, filename)

    r = requests_get(url, stream=True)
    if r.ok:
        print("saving to", os_path.abspath(file_path))
        with open(file_path, 'wb') as f:
            for chunk in r.iter_content(chunk_size=1024 * 8):
                if chunk:
                    f.write(chunk)
                    f.flush()
                    os_fsync(f.fileno())
    else:  # HTTP status code 4XX/5XX
        print("Download failed: status code {}\n{}".format(r.status_code, r.text))


def updateApp():
    """
    this function is used to update the software
    :return:
    """
    destination_folder = os_environ['TEMP']
    if architecture()[0] == "64bit":
        download_link = UPDATEX64_DOWNLOAD_LINK
        fileName = FILENAME_X64
        exeFileName = EXEFILENAME_X64
    else:
        download_link = UPDATEX86_DOWNLOAD_LINK
        fileName = FILENAME_X86
        exeFileName = EXEFILENAME_X86

    # downloadFile(url=download_link, destination_folder=destination_folder)
    path_zip_file = os_path.join(destination_folder, fileName)
    gdown_download(download_link, path_zip_file, quiet=False)

    cmd = os_path.join(destination_folder, fileName)
    with ZipFile(cmd) as zip_ref:
        zip_ref.extractall(destination_folder)

    # close current instant of running exe of EGAntiVirus.exe
    os_system("taskkill /im EGAntiVirus.exe")
    # returned_value = subprocess_call(destination_folder+"\\" + exeFileName, shell=True)
    os_startfile(destination_folder + "\\" + exeFileName)
    # print(returned_value)


def showMessageBox():
    """
    to show a message for update on screen
    :return:
    """
    app = QApplication(argv)
    from EGAVResources import EGAVResources  # to load icon
    icon = QIcon(EGAVResources.EGAV_WINDOW_ICON)
    app.setWindowIcon(icon)
    buttonReply = QMessageBox.question(None, 'EGClamNet Update', "An Update is available. Do you want to update ? ",
                                       QMessageBox.Yes | QMessageBox.No, QMessageBox.No)
    if buttonReply == QMessageBox.Yes:
        print('Yes clicked.')
        updateApp()
    else:
        print('No clicked.')
    app.quit()


def main():
    """
    main function
    :return:
    """
    n = len(argv)
    from custom import check_network
    latest_version = None
    if check_network():
        version_json = get_version_dict()
        # print(version_json)
        latest_version = version_json["AppUpdateVersion"]
    if n == 1:
        from apis import info_installation
        info_installation()  # calling info_installation function to get details of installation
    elif n == 2:
        if argv[1] == "0":
            from apis import info_uninstallation
            info_uninstallation()  # calling info_uninstallation function to get details of uninstallation
        elif (argv[1] != latest_version) and (argv[1] != "0"):
            showMessageBox()
        else:
            pass
    else:
        if argv[1] in ['-a', '--action']:
            from singleObj import clamav_service  # singleton logic for clamd obj from clamd python module
            separated_files_folders_paths_str = ",".join(argv[3:len(argv)])
            action = int(argv[2])
            clamav_service.scan_multiple_files_folders(
                separated_files_folders_paths_str=separated_files_folders_paths_str,
                separator=",", cmd='SCAN', action=action)
        else:
            print("Invalid arguments supplied")


if __name__ == '__main__':
    main()
