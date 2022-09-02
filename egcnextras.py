# ------------------------*************--------------------------- #
# make exe file with below command
# pyinstaller.exe --onefile --name egcnextras.exe egcnextras.py --icon="E:\All\CODE\PYTHON\pyEGAV\Resources\egav.ico"
# ------------------------*************--------------------------- #

from zipfile import ZipFile
from platform import architecture
from PySide6.QtGui import QIcon
from PySide6.QtWidgets import QMessageBox, QApplication
from urllib3 import PoolManager
from json import loads as json_loads
from sys import argv
from os import path as os_path, makedirs as os_make_dirs, fsync as os_fsync, environ as os_environ, system as os_system
from requests import get as requests_get
from subprocess import call as subprocess_call

version_string_url = "https://drive.google.com/uc?export=download&id=1aTGtQry1jXm03L9thdTYQCbcyjiLaBME"

UpdateX64_Download_link = "https://drive.google.com/uc?export=download&id=1NeHpJEoNwPN9B6O0J-ZI7eF2G2B2QDrE"
UpdateX86_Download_link = "https://drive.google.com/uc?export=download&id=1fgyOIp0thaxLkFsRThjvbNEgAHnIYreY"

fileName_x64 = "EGCNAV_WINx64_UPDATE.zip"
fileName_x86 = "EGCNAV_WINx86_UPDATE.exe"
exeFileName_x64 = "EGCNAV_WINx64_UPDATE.exe"
exeFileName_x86 = "EGCNAV_WINx86_UPDATE.exe"


def get_version_dict():
    try:
        http = PoolManager()
        resp = http.request('GET', version_string_url).data.decode("utf8")
        dict_value = json_loads(resp)
        return dict_value
        # print(resp.data.decode("utf8"))
    except Exception as e:
        print(e)
        return {}


def downloadFile(url: str, destination_folder: str):
    if not os_path.exists(destination_folder):
        os_make_dirs(destination_folder)  # create folder if it does not exist

    filename = destination_folder + "\\" + fileName_x64  # be careful with file names
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
    destination_folder = os_environ['TEMP']
    if architecture()[0] == "64bit":
        download_link = UpdateX64_Download_link
        fileName = fileName_x64
        exeFileName = exeFileName_x64
    else:
        download_link = UpdateX86_Download_link
        fileName = fileName_x86
        exeFileName = exeFileName_x86

    downloadFile(url=download_link, destination_folder=destination_folder)

    cmd = os_path.join(destination_folder, fileName)
    with ZipFile(cmd) as zip_ref:
        zip_ref.extractall(destination_folder)

    # close current instant of running exe of EGAntiVirus.exe
    os_system("taskkill /im EGAntiVirus.exe")
    returned_value = subprocess_call(destination_folder+"\\" + exeFileName, shell=True)
    print(returned_value)


def showMessageBox():
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
    n = len(argv)
    version_json = get_version_dict()
    # print(version_json)
    latest_version = version_json["AppUpdateVersion"]
    if n == 1:
        from custom import insertInstallationInfo  # calling function to insert details of an installation
        insertInstallationInfo()
    elif n == 2:
        if argv[1] != latest_version:
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
