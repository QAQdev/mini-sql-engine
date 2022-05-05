#ifndef MINISQL_LOG_MANAGER_H
#define MINISQL_LOG_MANAGER_H

/**
 * LogManager maintains a separate thread that is awakened whenever the
 * log buffer is full or whenever a timeout happens.
 * When the thread is awakened, the log buffer's content is written into the disk log file.
 *
 * Implemented by student self
 */
class LogManager {

};

#endif //MINISQL_LOG_MANAGER_H
