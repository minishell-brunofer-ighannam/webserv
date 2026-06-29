/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiProcess.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ighannam <ighannam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 23:18:30 by ighannam          #+#    #+#             */
/*   Updated: 2026/06/28 23:25:18 by ighannam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiProcess.hpp"
#include "SocketConnection.hpp"
#include "SocketPipeRead.hpp"
#include "SocketPipeWrite.hpp"

CgiProcess::CgiProcess(SocketConnection *client, const HttpRequest &req, SocketPipeWrite *stdin_pipe, SocketPipeRead *stdout_pipe, pid_t pid)
    : _client_conn(client), _stdin_pipe(stdin_pipe), _stdout_pipe(stdout_pipe), _child_pid(pid), _body_to_write(req.body), _body_write_offset(0),_stdout_buffer(), _start_time(time(NULL)), _stdout_closed(false), _request(req)
{
    
}

CgiProcess::~CgiProcess() {}

void CgiProcess::onStdinWritable() {}
void CgiProcess::onStdoutReadable() {}
bool CgiProcess::isDone() const { return _stdout_closed; }
bool CgiProcess::isExpired(time_t now, time_t timeout_secs) const
{
    return (now - _start_time) > timeout_secs;
}
void CgiProcess::kill() {}
void CgiProcess::buildAndSendResponse() {}