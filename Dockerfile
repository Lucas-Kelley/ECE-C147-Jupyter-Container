FROM ubuntu:22.04

RUN ln -snf /usr/share/zoneinfo/$CONTAINER_TIMEZONE /etc/localtime && echo $CONTAINER_TIMEZONE > /etc/timezone
RUN apt update && apt install -y python3 python3-pip pandoc texlive-xetex

COPY requirements.txt /src/pip_reqs/
RUN python3 -m pip install -r /src/pip_reqs/requirements.txt

RUN useradd -ms /bin/bash anon
COPY --chown=anon jupyter_server_config.json /home/anon/.jupyter/
USER anon

WORKDIR /notebook
ENTRYPOINT ["jupyter", "notebook", "--ip", "0.0.0.0"]
