// Created by Loris Witschard on 4/21/2018.
package barcode

enum class Action(val id: Int, val description: String)
{
    MOVE_FORWARD(0, "move ? units forward"),
    MOVE_BACKWARD(1, "move ? units backward"),
    ROTATE_LEFT(2, "rotate ?° left"),
    ROTATE_RIGHT(3, "rotate ?° right"),
    LED_ON(4, "turn LED ? on"),
    LED_OFF(5, "turn LED ? off"),
    WAIT(6, "wait for ? seconds"),
    END(7, "end of stream"),
    SYNC(8, "synchronisation")
}

data class Packet(private val action: Action, private val value: Int = 1)
{
    fun serialize(): Array<Boolean>
    {
        val code = when(action)
        {
            Action.MOVE_FORWARD, Action.MOVE_BACKWARD, Action.WAIT -> value - 1
            Action.ROTATE_LEFT, Action.ROTATE_RIGHT -> (value / 45) - 1
            Action.LED_ON, Action.LED_OFF -> value
            Action.END -> return arrayOf(true, true, true)
            Action.SYNC -> return arrayOf(true, false, true, false, true)
        }
        
        val bits = Array(6, { false })
        for(index in 0..2)
        {
            bits[index] = action.id shr index and 1 == 1
            bits[index + 3] = code shr index and 1 == 1
        }
        return bits
    }
    
    override fun toString(): String
    {
        return action.description.replace("?", value.toString())
    }
}
